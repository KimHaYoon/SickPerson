cd..
cd..

xcopy /s /y /d .\Engine\Include\*.h .\Island\Include\
xcopy /s /y /d .\Engine\Include\*.inl .\Island\Include\

xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64_Debug\Shader\*.* .\Engine\Bin64\Shader\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64\Shader\*.* .\Island\Bin64\Shader\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64_Debug\Shader\*.* .\Island\Bin64_Debug\Shader\


xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64_Debug\*.* .\Island\Bin64_Debug\
xcopy /s /y /d /exclude:Out.txt .\Engine\Bin64\*.* .\Island\Bin64\

xcopy /s /y /d .\Island\Bin64\*.* .\Client\Bin64\
xcopy /s /y /d .\Island\Bin64_Debug\*.* .\Client\Bin64_Debug\
