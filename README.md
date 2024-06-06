# IkunSnake
项目使用说明：将下列代码放在解决方案文件夹同级目录下，并将其命名为`Cola Props.props`
```
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ImportGroup Label="PropertySheets" />
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup>
    <OutDir>$(ProjectDir)bin\$(Platform) $(Configuration)\</OutDir>
    <IntDir>$(ProjectDir)temp\$(Platform) $(Configuration)\</IntDir>
    <_PropertySheetDisplayName>Cola Props</_PropertySheetDisplayName>
  </PropertyGroup>
  <ItemDefinitionGroup />
  <ItemGroup />
</Project>
```
