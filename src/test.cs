using System;
using System.Runtime.InteropServices;



public class HelloWorld
{
    [DllImport ("__Internal", EntryPoint=nameof(DoSomething))]
    static extern void DoSomething ();

    [DllImport ("__Internal", EntryPoint=nameof(PassInt))]
    static extern int PassInt (int i);

    [DllImport ("__Internal", EntryPoint=nameof(PassString))]
    static extern string PassString (string s);

    public static void Main(string[] args)
    {
        // DoSomething();
        // Console.WriteLine($"PassInt(3): {PassInt(3)}");
        // Console.WriteLine($"PassString(\"Hello\"): {PassString("Hello")}");
        // Console.WriteLine ("Hello Mono World");
    }

    public static int GetNumber(int num)
    {
        return num * 5;
    }
}

public class GameObject {
    public string name;
    private float x;
    public GameObject() {
        this.name = "No name";
        x = 0;
    }
    public GameObject(string name) {
        this.name = name;
    }
    public void Update(int dt) {
        x += dt;
        Console.WriteLine($"updating by {dt} seconds and is now {x}");
        Console.WriteLine($"{(float)dt}");
    }
}
// public class Dog
// {
//     static public void Type()
//     {
//         // Console.WriteLine("a Dog!");
//     }
// }