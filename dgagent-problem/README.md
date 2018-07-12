Reproducer for Digital Guardian agent problem related to application crash handling

Steps to reproduce:
* Download & unzip QtCrashReproducer_32bit.7z.
* Run QtCrashReproducer.exe in the QtCrashReproducer\debug folder.
* Click on the "Click to crash application" button.
* Observe that application does _not_ crash immediately as expected on systems with DG installed.

The reproducer performs an invalid operation (null-pointer read) when clicking on the button. This triggers an "access violation" exception that is supposed to immediately crash the application. However, that does not happen on systems with DG installed. The sourcecode for the reproducer is included in QtCrashReproducer_32bit.7z. Note that the reproducer is designed to illustrate a potential failure mode, and not representative for the actual failures observed.

The way DG interferes with application crash handling is problematic, because it leaves the application running in a corrupted state. For medical SW, this can potentially lead to incorrect patient diagnostics, with severe consequences. It's much better to let the application crash than to keep it running in a corrupted state.
