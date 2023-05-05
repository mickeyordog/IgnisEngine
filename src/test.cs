using System;
using System.Runtime.InteropServices;



public class HelloWorld
{
    [DllImport ("__Internal", EntryPoint="DoSomething")]
    static extern void DoSomething ();
    
    public static void Main(string[] args)
    {
        DoSomething();
        Console.WriteLine ("Hello Mono World");
    }
}
// public class Dog
// {
//     static public void Type()
//     {
//         // Console.WriteLine("a Dog!");
//     }
// }