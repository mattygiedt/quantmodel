
;--------------------------------

; The name of the installer
Name "QuantModel Account Viewer"

; The file to write
OutFile "installer.exe"

; The default installation directory
InstallDir $PROGRAMFILES\QuantModel\Viewer

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\QM_VIEWER" "Install_Dir"

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
Section "QuantModel Account Viewer"

  SectionIn RO

  SetOutPath $INSTDIR

  File "viewer.exe"
  File "protobuf.dll"
  File "Google.ProtocolBuffers.dll"
  File "libzmq.dll"
  File "viewer.exe.config"
  File "Eyefinder.dll"
  File "System.Windows.Forms.DataVisualization.dll"
  File "log4net.dll"
  File "common.dll"
  File "CheckBoxComboBox.dll"
  File "XPExplorerBar.dll"
  File "GlacialTreeList.dll"
  File "ZeroMQ.dll"

  ; Create the desktop shortcut and start-menu items
  CreateShortCut "$DESKTOP\quantmodel.lnk" "$INSTDIR\viewer.exe" ""
  CreateDirectory "$SMPROGRAMS\QuantModel\Viewer"
  CreateShortCut "$SMPROGRAMS\QuantModel\Viewer\uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\QuantModel\Viewer\quantmodel.lnk" "$INSTDIR\viewer.exe" "" "$INSTDIR\viewer.exe" 0

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\QM_VIEWER "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_VIEWER" "DisplayName" "QuantModel Account Viewer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_VIEWER" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_VIEWER" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_VIEWER" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QM_VIEWER"
  DeleteRegKey HKLM SOFTWARE\QM_VIEWER

  ; Remove Start Menu items
  Delete "$SMPROGRAMS\QuantModel\Viewer\*.*"
  RmDir  "$SMPROGRAMS\QuantModel\Viewer"

  ; Remove files and uninstaller
  Delete "$DESKTOP\quantmodel.lnk"
  Delete "$INSTDIR\*.*"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd
