#!/usr/bin/env python
from avatar2 import *
from avatar2.message import *
from time import sleep
from multiprocessing import Process, Queue
import logging
import os

import hybridFuzz.fuzz as fuzz
import hybridFuzz.utils as utils
import hybridFuzz.peripheral as peripheral


import proj.proj_hjx_new.frdmk66f_lwip_httpssrv_mbedTLS_freertos.firmConf as conf              #load firmware specific configuration
# import proj.Demo.stackobject_plugin.firmConf as conf

loop_times = 10

logger = logging.getLogger('app')
logger.setLevel(logging.DEBUG)


ROM = bytearray()

TARGET="qemu"

def before_strat_pc(avatar, remote_memory_msg, **kwargs):
    while(avatar.targets[TARGET].state != TargetStates.STOPPED):
        continue
    if (conf.START_PC == avatar.targets[TARGET].read_register('pc')):
        print("save snapshot",conf.SNAPSHOT_NAME, "before start pc 0x%x", conf.START_PC,"...")
        avatar.targets[TARGET].saveVMSnapshot(conf.SNAPSHOT_NAME)
        ret = avatar.targets[TARGET].infoVMSnapshot()
        if (conf.SNAPSHOT_NAME in ret):
            print("successfully saved")

def after_end_pc(avatar, remote_memory_msg, **kwargs):
    while (avatar.targets[TARGET].state != TargetStates.STOPPED):
        continue
    if (conf.END_PC == avatar.targets[TARGET].read_register('pc')):
        print("end running after end pc 0x%x ..."%conf.END_PC)
        fuzz.gvfw.close()
        tt = time.time() - fuzz.total_start_time
        fuzz.update_gpq(conf.GV_FILE)
        print('Child process', os.getpid(), 'end pc')
        avatar.q.put(os.getpid())
        avatar.q.put(tt)
        avatar.q.put(fuzz.total_path)
        sleep(10)

# end running after target exited
def updateState_after_cb(avatar, *args, **kwargs):
    if (avatar.targets[TARGET].state == TargetStates.EXITED):
        fuzz.gvfw.close()
        tt = time.time() - fuzz.total_start_time
        fuzz.update_gpq(conf.GV_FILE)
        print('Child process', os.getpid(), 'end exited')
        avatar.q.put(os.getpid())
        avatar.q.put(tt)
        avatar.q.put(fuzz.total_path)
        sleep(10)
    return


def run(q):
    # conf.FORWAED_DEPTH=fd
    print(conf.FORWAED_DEPTH, 'Parent process:', os.getppid(), 'Child process:', os.getpid())
    print("[+] Initializing the global_peripheral_queue")

    # Create avatar instance with custom output directory
    avatar = Avatar(arch=archs.arm.ARM_CORTEX_M3, output_directory=conf.OUT_DIR)
    avatar.watchmen.add_watchman('UpdateState', 'after', updateState_after_cb)
    avatar.q = q

    # Get symbols
    funcs = utils.get_symbols(conf.SYMBOL_FILE)
    # GV File
    if (conf.EN_BB2BB):
        bb2bb = conf.BB2BB
    else:
        bb2bb = 0
    if (conf.EN_ONE_RUN_TIME):
        ort = conf.ONE_RUN_TIME
    else:
        ort = 0
    peripheral.start_exec_time = time.time()
    fuzz.init_gpq(ort, bb2bb, conf.GV_FILE, conf.GRAPH_DIR, conf.LOG_DIR, peripheral.start_exec_time, q)

    fuzz.gvfw.write("---------------New loop---------------\n")

    plugin_args = {}
    if conf.PLUGIN == "tcg-plugin-hook_network.so":
        plugin_args["NET_FUN_ADDR"] = funcs.get(conf.NET_FUNC, 0)
        plugin_args["SEED_PATH"] = os.path.join(conf.PROJ_PATH, "seed")
        plugin_args["BUF_REG_INDEX"] = conf.BUF_REG_INDEX
        plugin_args["LEN_REG_INDEX"] = conf.LEN_REG_INDEX
        plugin_args["GVFILE_PATH"] = conf.GV_FILE
        plugin_args["IS_MUT"] = "true" if conf.IS_MUT else "false"
    if conf.PLUGIN == "tcg-plugin-heapobject_tracking.so":
        for f in conf.HEAP_FUNCS:
            plugin_args[f] = funcs.get(f, 0)
        for f in conf.HEAP_FUNCS_R:
            plugin_args[f[1:]] = funcs.get(f, 0)
    if conf.PLUGIN == "tcg-plugin-segment_tracking.so":
        plugin_args["CONF_FILE"] = os.path.join(conf.OUT_DIR, "conf.json")

    # Add panda target
    logger.info("[+] Creating the QEMUTarget")
    qemu = avatar.add_target(QemuTarget, name="qemu",
                              gdb_executable="arm-none-eabi-gdb",
                              firmware=conf.SAMPLE, cpu_model="cortex-m4",
                              executable=conf.QEMU_PATH,
                              drive_qcow2=conf.QCOW2_Path,
                              plugins=conf.PLUGIN,
                              plugin_args=plugin_args,
                              raw=conf.RAW_BIBARY,
                              interval=conf.INTERVAL)

    # Add angr target
    logger.info("[+] Creating the AngrTarget")
    angr = avatar.add_target(AngrTarget, binary=conf.SAMPLE, ram_file=conf.RAM_FILE, name='angr', \
                             load_options={'main_opts': {'backend': 'blob', 'custom_arch': 'ARM', \
                                                         'custom_base_addr': conf.LOAD_OFFSET,
                                                         'custom_entry_point': conf.ENTRY_POINT}})

    with open(conf.SAMPLE, "rb") as binary_file:
        # Read the whole file at once
        ROM = binary_file.read()

    # add memory
    for ramfile in conf.RAM_FILE:
        avatar.add_memory_range(ramfile['start'], ramfile['size'], name="ram" + ramfile['file'],
                                permissions='rw-')
    rom = avatar.add_memory_range(conf.ROM_FILE['start'], conf.ROM_FILE['size'], name='rom',
                                  file=conf.SAMPLE,
                                  permissions='r-x', alias=conf.ROM_FILE['alias'])
    if conf.EN_BOOT_LOADER:
        bootloader = avatar.add_memory_range(conf.BOOT_LOADER_START, conf.BOOT_LOADER_SIZE,
                                         name='bootloader',
                                         file=conf.SAMPLE,
                                         permissions='r-x', alias=conf.ROM_FILE['alias'])

    # setting unknown peripheral memory range
    IgnorePeripheralList = {
        "unknown-peripheral": (conf.UNKNOWN_PERIPHERAL_MEM_START, conf.UNKNOWN_PERIPHERAL_MEM_SIZE),
    }

    for name, addr in IgnorePeripheralList.items():
        avatar.add_memory_range(addr[0], addr[1], name=name, emulate=peripheral.IgnorePeripheral,
                                rom=ROM, rom_offset=conf.ROM_START,
                                stopHooks=conf.STOP_HOOKS,
                                qemu_target=qemu,
                                angr_target=angr,
                                chip_specific=conf.CHIP_INFO,
                                alg=conf.PATH_SELECTION_MODE,
                                asserts=conf.ASSERT_FUNC,
                                fixedPeriV=conf.FIXED_PERIPHERAL,
                                debug_port=conf.DEBUG_PORT,
                                manual_path=conf.MANUAL_PATH,
                                pri_periph = conf.PRIVATE_PERIPHERAL,
                                forward_depth=conf.FORWAED_DEPTH,
                                depth=conf.CONTEX_DEPTH,
                                his=conf.HIS,
                                permissions='rw-')

    # start
    logger.info("[+] Initializing the targets")
    avatar.init_targets()

    # -- -- #
    ret = qemu.infoVMSnapshot()
    if (conf.EN_START_PC and conf.SNAPSHOT_NAME in ret):
        qemu.loadVMSnapshot(conf.SNAPSHOT_NAME)
        print("load snapshot", conf.SNAPSHOT_NAME)
        print("start running from start pc 0x%x"%conf.START_PC)
    elif (conf.EN_START_PC):
        qemu.set_breakpoint(conf.START_PC)
        print("set breakpoint on start pc 0x%x"%conf.START_PC)
        avatar.watchmen.add_watchman('BreakpointHit', 'before', before_strat_pc, is_async=False)

    if (conf.EN_END_PC):
        qemu.set_breakpoint(conf.END_PC)
        print("set breakpoint on end pc 0x%x"%conf.END_PC)
        avatar.watchmen.add_watchman('BreakpointHit', 'before', after_end_pc, is_async=False)


    logger.info("[+] Running in QEMU until a peripherial is accessed")

    while True:
        qemu.cont()
        qemu.wait()
        sleep(3)


if __name__ == "__main__":
    if conf.CREATE_SAMPLE:
        res = os.system("../scratch/convertRaw.sh " + conf.SYMBOL_FILE)
        if res != 0:
            exit()
    if conf.CREATE_QCOW2:
        res = os.system("qemu-img convert -f raw -O qcow2 " + conf.SYMBOL_FILE + " " + conf.QCOW2_Path)
        if res != 0:
            exit()
    fuzz.clear_gpq()
    os.system("rm -rf {0}".format(os.path.join(conf.PROJ_PATH, "seed")))
    print('rm myavatar', os.system('rm -rf %s' % conf.OUT_DIR))
    print('Parent process:', os.getpid())
    rt = 0
    bb = 0
    # fd=1
    while (True):
        if loop_times <= 0:
            break
        loop_times = loop_times - 1
        q = Queue()
        # fd=fd+1
        child = Process(target=run, args=(q, ))
        child.start()
        spid = q.get()
        rt = q.get()
        bb = q.get()
        q.close()
        print('kill qemu', os.system("ps -C qemu-system-arm -o pid=|xargs kill -9"))
        print('kill', spid, os.system('kill %d' % spid))
        # sleep(1)
        # if (conf.EN_MULTIPLE_RUN_TIME and rt < conf.RUN_TIME):
        #     if (conf.EN_BB2BB and bb >= conf.BB2BB):
        #         break
        # else:
        #     break

    fuzz.clear_gpq()
    print('Parent process', os.getpid(), "end")