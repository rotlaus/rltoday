This is by no means complete. There are too many WM devices for this to be accurate for them all. Y = confirmed yes, N = confirmed no, blank = unconfirmed. Note that a confirmed no may be inaccurate due to third-party software or relience on optional hardware.

| Name                | File path:                    | Command Line/Comments:                         | WM2003 | WM5 | WM6 |
|:--------------------|:------------------------------|:-----------------------------------------------|:-------|:----|:----|
| Beam/IR recive on   | \windows\beam.exe or \windows\irsquirt.exe | Needs commands in WM5/6                        |    Y   |  Y  |  Y  |
| Bluetooth Manager   | \windows\Bluetoothmanager.exe |                                                |        |     |     |
| Bluetooth Settings  | \Windows\BTSetting.exe        | Not the control panel                          |    Y   |  N  |  N  |
| Calculator          | \Windows\calc.exe             |                                                |    Y   |  Y  |  N  |
| Calculator          | \Windows\calculator.exe       |                                                |    N   |  N  |  Y  |
| Calculator          | MobileCalculator.exe          | For the Samsung i760 \windows not needed       |      |     |     |
| Call History        | \Windows\LaunchMSItem.exe     | CALLHISTORY\_ALL\_CALLS\\ CALLHISTORY\_INCOMING\\ CALLHISTORY\_OUTGOING\\ CALLHISTORY\_MISSED                                                                                     |    N   |  N  |  Y  |
| Camera              | \windows\Camera.exe           |                                                |    Y   |  N  |  Y  |
| Camera              | \windows\CameraApp.exe        |                                                |    N   |  Y  |  N  |
| Clock and Alarms    | \windows\clock.exe            |                                                |    Y   |  Y  |  Y  |
| Contacts            | \windows\addrbook.exe         |                                                |    Y   |  N  |  N  |
| Dial Pad            | \windows\cprog.exe            | -n -url tel:+316434407                       |    Y   |  Y  |  Y  |
| Excel Mobile        | \Windows\pxl.exe              | Document path as Command Line is possible      |    Y   |  Y  |  Y  |
| File explorer       | \windows\fexplore.exe         |                                                |    Y   |  Y  |  Y  |
| Find aka Search     | \windows\shfind.exe           |                                                |    Y   |  Y  |  Y  |
| Help                | \windows\peghelp.exe          | Used to display .htp files (supporting topics) |    Y   |  Y  |  Y  |
| Internet Explorer   | \Windows\iexplore.exe         | Document path as Command Line is possible      |    Y   |  Y  |  Y  |
| iMobi Java          | \Windows\Midp2.exe            |                                                |    N   |  Y  |  N  |
| ipaq backup         | \windows\ipaq backup.exe      |                                                |        |     |     |
| ipaq itask          | \windows\itask.exe            |                                                |        |     |     |
| Ipaq Wireless wm3   | \windows\wirelesscenter.exe   |                                                |        |     |     |
| Media Player        | \Windows\wmplayer.exe         | Document path as Command Line is possible      |    Y   |  Y  |  Y  |
| Messaging           | \Windows\LaunchMSItem.exe     | MESSAGING\_EMAIL\_INBOX\\ MESSAGING\_EMAIL\_NEW\\ MESSAGING\_SMS\_INBOX\\ MESSAGING\_SMS\_NEW                                                                                      |    N   |  N  |  Y  |
| Messaging           | \windows\tmail.exe            | -service, -sync, -cc, -bcc, -transport, -to, -subject, -body, -attach                                                                                                 |    Y   |  Y  |  Y  |
| MIDlet Manager      | \windows\jmm.exe              | -r

&lt;suite&gt;

,

&lt;vendor&gt;

,

&lt;name&gt;

\\ 

&lt;suite&gt;

, 

&lt;vendor&gt;

, and 

&lt;name&gt;

 can be obtained from the relevant MIDlet line in the jad file.                                                |        |     |     |
| Ms Reader           | \windows\Msreader.exe         |                                                |        |     |     |
| Msn Messenger       | \windows\instmsgr.exe         |                                                |    Y   |  Y  |  N  |
| Notes               | \windows\notes.exe            |                                                |    Y   |  Y  |  N  |
| Pictures and Videos | \windows\pimg.exe             |                                                |    Y   |  Y  |  Y  |
| Pocket Excel        | \Windows\pxl.exe              | Document path as Command Line is possible      |    Y   |  Y  |  Y  |
| Pocket Word         | \Windows\pword.exe            | Document path as Command Line is possible      |    Y   |  Y  |  Y  |
| Search aka Find     | \windows\shfind.exe           |                                                |    Y   |  Y  |  Y  |
| self test           | \windows\selftest.exe         |                                                |        |     |     |
| Solitaire           | \windows\solitare.exe         |                                                |    Y   |  Y  |  Y  |
| Speed Dial Pad      | \windows\tspeeddial.exe       |                                                |        |     |     |
| Tasks               | \windows\tasks.exe            |                                                |    Y   |  Y  |  Y  |
| Terminal Services   | \windows\mstsc40.exe          |                                                |    Y   |  Y  |  N  |
| Word Mobile         | \Windows\pword.exe            | Document path as Command Line is possible      |    Y   |  Y  |  Y  |

**For example, File path: \windows\tmail.exe Command line: -service "SMS"**


#### Control pannel cpl's ####
| Name               | File path:             | WM2003 | WM5 | WM6 |
|:-------------------|:-----------------------|:-------|:----|:----|
| backlight          | \windows\backlight.cpl |   Y    |  Y  |  Y  |
| BT config          | \windows\btconfigCE.cpl|        |     |     |
| Clear type tuner   | \windows\cttune.cpl    |        |     |     |
| Expansion pack info| \windows\epcpl.cpl     |        |     |     |
| ipaq audio         | \windows\ipaqaudio.cpl |        |     |     |
| ipaq bio swipe     | \windows\bioswipe.cpl  |        |     |     |
| Itask options      | \windows\itask.cpl     |        |     |     |
| Memory             | \windows\cplmain.cpl   |   Y    |  Y  |  Y  |
| Power options      | \windows\power.cpl     |   Y    |  Y  |  Y  |

cplmain.cpl includes standard applets.