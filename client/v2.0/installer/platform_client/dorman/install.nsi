
;--------------------------------

; The name of the installer
Name "QuantModel Platform Client"

; The file to write
OutFile "installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\QuantModel\Client

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\QM_CLIENT" "Install_Dir"

LicenseText "QuantModel Software License Agreement"
LicenseData "..\..\license.txt"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page license
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

; The stuff to install
Section "QuantModel Platform Client"

  SectionIn RO

  SetOutPath $INSTDIR

  File "client.exe"
  File "protobuf.dll"
  File "Google.ProtocolBuffers.dll"
  File "libzmq.dll"
  File "client.exe.config"
  File "Eyefinder.dll"
  File "System.Windows.Forms.DataVisualization.dll"
  File "log4net.dll"
  File "common.dll"
  File "CheckBoxComboBox.dll"
  File "XPExplorerBar.dll"
  File "GlacialTreeList.dll"
  File "ZeroMQ.dll"

  ; Create the desktop shortcut and start-menu items
  CreateShortCut "$DESKTOP\quantmodel.lnk" "$INSTDIR\client.exe" ""
  CreateDirectory "$SMPROGRAMS\QuantModel\Client"
  CreateShortCut "$SMPROGRAMS\QuantModel\Client\uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\QuantModel\Client\quantmodel.lnk" "$INSTDIR\client.exe" "" "$INSTDIR\client.exe" 0

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\QM_CLIENT "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_CLIENT" "DisplayName" "QuantModel Platform Client"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_CLIENT" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_CLIENT" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_CLIENT" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_CLIENT"
  DeleteRegKey HKLM SOFTWARE\QM_CLIENT

  ; Remove Start Menu items
  Delete "$SMPROGRAMS\QuantModel\Client\*.*"
  RmDir  "$SMPROGRAMS\QuantModel\Client"

  ; Remove files and uninstaller
  Delete "$DESKTOP\quantmodel.lnk"
  Delete "$INSTDIR\*.*"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd
