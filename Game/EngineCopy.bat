cd..
cd..

xcopy /s /y /d .\Engine\Include\*.h .\GameEngine\Include\
xcopy /s /y /d .\Engine\Include\*.inl .\GameEngine\Include\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin_Debug\*.* .\GameEngine\Bin_Debug\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64_Debug\*.* .\GameEngine\Bin64_Debug\

xcopy /s /y /d .\GameEngine\Bin_Debug\*.* .\Client\Bin_Debug\
xcopy /s /y /d .\GameEngine\Bin64_Debug\*.* .\Client\Bin64_Debug\