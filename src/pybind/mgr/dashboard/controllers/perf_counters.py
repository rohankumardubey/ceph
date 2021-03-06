# -*- coding: utf-8 -*-

import cherrypy

from .. import mgr
from ..security import Scope
from ..services.ceph_service import CephService
from . import ApiController, ControllerDoc, EndpointDoc, RESTController

PERF_SCHEMA = {
    "mon.a": ({
        ".cache_bytes": ({
            "description": (str, ""),
            "nick": (str, ""),
            "type": (int, ""),
            "priority": (int, ""),
            "units": (int, ""),
            "value": (int, "")
        }, ""),
    }, "Service ID"),
}


class PerfCounter(RESTController):
    service_type = None  # type: str

    def get(self, service_id):
        try:
            return CephService.get_service_perf_counters(self.service_type, str(service_id))
        except KeyError as error:
            raise cherrypy.HTTPError(404, "{0} not found".format(error))


@ApiController('perf_counters/mds', Scope.CEPHFS)
@ControllerDoc("Mds Perf Counters Management API", "MdsPerfCounter")
class MdsPerfCounter(PerfCounter):
    service_type = 'mds'


@ApiController('perf_counters/mon', Scope.MONITOR)
@ControllerDoc("Mon Perf Counters Management API", "MonPerfCounter")
class MonPerfCounter(PerfCounter):
    service_type = 'mon'


@ApiController('perf_counters/osd', Scope.OSD)
@ControllerDoc("OSD Perf Counters Management API", "OsdPerfCounter")
class OsdPerfCounter(PerfCounter):
    service_type = 'osd'


@ApiController('perf_counters/rgw', Scope.RGW)
@ControllerDoc("Rgw Perf Counters Management API", "RgwPerfCounter")
class RgwPerfCounter(PerfCounter):
    service_type = 'rgw'


@ApiController('perf_counters/rbd-mirror', Scope.RBD_MIRRORING)
@ControllerDoc("Rgw Mirroring Perf Counters Management API", "RgwMirrorPerfCounter")
class RbdMirrorPerfCounter(PerfCounter):
    service_type = 'rbd-mirror'


@ApiController('perf_counters/mgr', Scope.MANAGER)
@ControllerDoc("Mgr Perf Counters Management API", "MgrPerfCounter")
class MgrPerfCounter(PerfCounter):
    service_type = 'mgr'


@ApiController('perf_counters/tcmu-runner', Scope.ISCSI)
@ControllerDoc("Tcmu Runner Perf Counters Management API", "TcmuRunnerPerfCounter")
class TcmuRunnerPerfCounter(PerfCounter):
    service_type = 'tcmu-runner'


@ApiController('perf_counters')
@ControllerDoc("Perf Counters Management API", "PerfCounters")
class PerfCounters(RESTController):
    @EndpointDoc("Display Perf Counters",
                 responses={200: PERF_SCHEMA})
    def list(self):
        return mgr.get_all_perf_counters()
