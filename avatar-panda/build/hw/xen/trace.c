/* This file is autogenerated by tracetool, do not edit. */

#include "qemu/osdep.h"
#include "trace.h"

uint16_t _TRACE_XEN_DEFAULT_IOREQ_SERVER_DSTATE;
uint16_t _TRACE_XEN_IOREQ_SERVER_CREATE_DSTATE;
uint16_t _TRACE_XEN_IOREQ_SERVER_DESTROY_DSTATE;
uint16_t _TRACE_XEN_IOREQ_SERVER_STATE_DSTATE;
uint16_t _TRACE_XEN_MAP_MMIO_RANGE_DSTATE;
uint16_t _TRACE_XEN_UNMAP_MMIO_RANGE_DSTATE;
uint16_t _TRACE_XEN_MAP_PORTIO_RANGE_DSTATE;
uint16_t _TRACE_XEN_UNMAP_PORTIO_RANGE_DSTATE;
uint16_t _TRACE_XEN_MAP_PCIDEV_DSTATE;
uint16_t _TRACE_XEN_UNMAP_PCIDEV_DSTATE;
TraceEvent _TRACE_XEN_DEFAULT_IOREQ_SERVER_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_default_ioreq_server",
    .sstate = TRACE_XEN_DEFAULT_IOREQ_SERVER_ENABLED,
    .dstate = &_TRACE_XEN_DEFAULT_IOREQ_SERVER_DSTATE 
};
TraceEvent _TRACE_XEN_IOREQ_SERVER_CREATE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_ioreq_server_create",
    .sstate = TRACE_XEN_IOREQ_SERVER_CREATE_ENABLED,
    .dstate = &_TRACE_XEN_IOREQ_SERVER_CREATE_DSTATE 
};
TraceEvent _TRACE_XEN_IOREQ_SERVER_DESTROY_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_ioreq_server_destroy",
    .sstate = TRACE_XEN_IOREQ_SERVER_DESTROY_ENABLED,
    .dstate = &_TRACE_XEN_IOREQ_SERVER_DESTROY_DSTATE 
};
TraceEvent _TRACE_XEN_IOREQ_SERVER_STATE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_ioreq_server_state",
    .sstate = TRACE_XEN_IOREQ_SERVER_STATE_ENABLED,
    .dstate = &_TRACE_XEN_IOREQ_SERVER_STATE_DSTATE 
};
TraceEvent _TRACE_XEN_MAP_MMIO_RANGE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_map_mmio_range",
    .sstate = TRACE_XEN_MAP_MMIO_RANGE_ENABLED,
    .dstate = &_TRACE_XEN_MAP_MMIO_RANGE_DSTATE 
};
TraceEvent _TRACE_XEN_UNMAP_MMIO_RANGE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_unmap_mmio_range",
    .sstate = TRACE_XEN_UNMAP_MMIO_RANGE_ENABLED,
    .dstate = &_TRACE_XEN_UNMAP_MMIO_RANGE_DSTATE 
};
TraceEvent _TRACE_XEN_MAP_PORTIO_RANGE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_map_portio_range",
    .sstate = TRACE_XEN_MAP_PORTIO_RANGE_ENABLED,
    .dstate = &_TRACE_XEN_MAP_PORTIO_RANGE_DSTATE 
};
TraceEvent _TRACE_XEN_UNMAP_PORTIO_RANGE_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_unmap_portio_range",
    .sstate = TRACE_XEN_UNMAP_PORTIO_RANGE_ENABLED,
    .dstate = &_TRACE_XEN_UNMAP_PORTIO_RANGE_DSTATE 
};
TraceEvent _TRACE_XEN_MAP_PCIDEV_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_map_pcidev",
    .sstate = TRACE_XEN_MAP_PCIDEV_ENABLED,
    .dstate = &_TRACE_XEN_MAP_PCIDEV_DSTATE 
};
TraceEvent _TRACE_XEN_UNMAP_PCIDEV_EVENT = {
    .id = 0,
    .vcpu_id = TRACE_VCPU_EVENT_NONE,
    .name = "xen_unmap_pcidev",
    .sstate = TRACE_XEN_UNMAP_PCIDEV_ENABLED,
    .dstate = &_TRACE_XEN_UNMAP_PCIDEV_DSTATE 
};
TraceEvent *hw_xen_trace_events[] = {
    &_TRACE_XEN_DEFAULT_IOREQ_SERVER_EVENT,
    &_TRACE_XEN_IOREQ_SERVER_CREATE_EVENT,
    &_TRACE_XEN_IOREQ_SERVER_DESTROY_EVENT,
    &_TRACE_XEN_IOREQ_SERVER_STATE_EVENT,
    &_TRACE_XEN_MAP_MMIO_RANGE_EVENT,
    &_TRACE_XEN_UNMAP_MMIO_RANGE_EVENT,
    &_TRACE_XEN_MAP_PORTIO_RANGE_EVENT,
    &_TRACE_XEN_UNMAP_PORTIO_RANGE_EVENT,
    &_TRACE_XEN_MAP_PCIDEV_EVENT,
    &_TRACE_XEN_UNMAP_PCIDEV_EVENT,
  NULL,
};

static void trace_hw_xen_register_events(void)
{
    trace_event_register_group(hw_xen_trace_events);
}
trace_init(trace_hw_xen_register_events)
