### [UnrealEngine.Framework](./UnrealEngine-Framework.md 'UnrealEngine.Framework').[InputComponent](./InputComponent.md 'UnrealEngine.Framework.InputComponent')
## InputComponent.BindAxis(string, UnrealEngine.Framework.InputAxisDelegate, bool) Method
Binds a static callback function an axis defined in the project settings or by using [AddAxisMapping(string, string, float)](./Engine-AddAxisMapping(string_string_float).md 'UnrealEngine.Framework.Engine.AddAxisMapping(string, string, float)')  
```csharp
public void BindAxis(string axisName, UnrealEngine.Framework.InputAxisDelegate callback, bool executedWhenPaused=false);
```
#### Parameters
<a name='UnrealEngine-Framework-InputComponent-BindAxis(string_UnrealEngine-Framework-InputAxisDelegate_bool)-axisName'></a>
`axisName` [System.String](https://docs.microsoft.com/en-us/dotnet/api/System.String 'System.String')  
The name of the axis  
  
<a name='UnrealEngine-Framework-InputComponent-BindAxis(string_UnrealEngine-Framework-InputAxisDelegate_bool)-callback'></a>
`callback` [InputAxisDelegate(float)](./InputAxisDelegate(float).md 'UnrealEngine.Framework.InputAxisDelegate(float)')  
The static function to call while tracking axis  
  
<a name='UnrealEngine-Framework-InputComponent-BindAxis(string_UnrealEngine-Framework-InputAxisDelegate_bool)-executedWhenPaused'></a>
`executedWhenPaused` [System.Boolean](https://docs.microsoft.com/en-us/dotnet/api/System.Boolean 'System.Boolean')  
If `true`, executes even if the game is paused  
  
#### Exceptions
[System.ArgumentException](https://docs.microsoft.com/en-us/dotnet/api/System.ArgumentException 'System.ArgumentException')  
Thrown if [callback](#UnrealEngine-Framework-InputComponent-BindAxis(string_UnrealEngine-Framework-InputAxisDelegate_bool)-callback 'UnrealEngine.Framework.InputComponent.BindAxis(string, UnrealEngine.Framework.InputAxisDelegate, bool).callback') is not static  
