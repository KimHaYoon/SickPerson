cd..
cd..

xcopy /s /y /d .\GameEngine\Include\*.h .\Engine\Include\
xcopy /s /y /d .\GameEngine\Include\*.inl .\Engine\Include\

xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin_Debug\*.* .\Engine\Bin_Debug\
xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin64_Debug\*.* .\Engine\Bin64_Debug\
xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin\*.* .\Engine\Bin\
xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin64\*.* .\Engine\Bin64\

xcopy /s /y /d .\Engine\Bin_Debug\*.* .\Client\Bin_Debug\
xcopy /s /y /d .\Engine\Bin\*.* .\Client\Bin\
xcopy /s /y /d .\Engine\Bin64\*.* .\Client\Bin64\
xcopy /s /y /d .\Engine\Bin64_Debug\*.* .\Client\Bin64_Debug\

xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin64_Debug\Shader\*.* .\GameEngine\Bin\Shader\
xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin64_Debug\Shader\*.* .\GameEngine\Bin64\Shader\
xcopy /s /y /d /exclude:Out.txt .\GameEngine\Bin64_Debug\Shader\*.* .\GameEngine\Bin_Debug\Shader\

xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Mesh\*.* .\Client\Bin\Mesh\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Mesh\*.* .\Client\Bin64\Mesh\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Mesh\*.* .\Client\Bin64_Debug\Mesh\

xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Texture\*.* .\Client\Bin\Texture\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Texture\*.* .\Client\Bin64\Texture\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Texture\*.* .\Client\Bin64_Debug\Texture\

xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Data\*.* .\Client\Bin\Data\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Data\*.* .\Client\Bin64\Data\
xcopy /s /y /d /exclude:Out.txt .\Client\Bin_Debug\Data\*.* .\Client\Bin64_Debug\Data\