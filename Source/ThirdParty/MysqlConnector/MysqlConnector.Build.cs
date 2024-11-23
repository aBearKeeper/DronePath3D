
using UnrealBuildTool;
using System.IO;

public class MysqlConnector : ModuleRules
{
   public MysqlConnector(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;
        //库文件路径
        PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
        //头文件路径
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
        //添加静态库
        PublicSystemLibraries.Add(Path.Combine(ModuleDirectory, "lib", "mysqlcppconn-static.lib"));
        //加入动态库
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "lib", "libcrypto-3-x64.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "lib", "libssl-3-x64.dll"));
    }
}
