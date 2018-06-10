# Check for admin permissions before anything else
import ctypes
if not ctypes.windll.shell32.IsUserAnAdmin():
    raise RuntimeError("Script MUST be run with admin rights")

import comtypes
import comtypes.client
import comtypes.server.localserver
import comtypes.server.register

# Load type library
PyComServer = comtypes.client.GetModule("PyComServer.tlb")

class MyComClass (PyComServer.MyComClass):
    # registry entries
    _reg_threading_ = "Both"
    _reg_progid_ = "PyComServer.MyComClass.1"
    _reg_novers_progid_ = "PyComServer.MyComClass"
    _reg_desc_ = "Simple COM server for testing"
    _reg_clsctx_ = comtypes.CLSCTX_INPROC_SERVER | comtypes.CLSCTX_LOCAL_SERVER
    _regcls_ = comtypes.server.localserver.REGCLS_MULTIPLEUSE

    def __init__(self, *args, **kwargs):
        super(MyComClass, self).__init__(*args, **kwargs)


if __name__ == "__main__":
    # Register COM class
    comtypes.server.register.Registrar().register(MyComClass)
    
    # Instanciate COM class (fails with 64bit Python 3)
    obj = comtypes.client.CreateObject(MyComClass)
    print(obj)
    
    print("Success")
