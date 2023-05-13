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
        name = "no name";
        x = 5.3f;
        // Console.WriteLine($"Constructing {name} with x {x}");

    }
    public GameObject(string name) {
        this.name = name;
        x = 0;
        Console.WriteLine($"Constructing {name} with x {x}");

    }
    public GameObject(float x) {
        name = "no name";
        this.x = x;
        Console.WriteLine($"Constructing {name} with x {x}");
    }
    public GameObject(string name, float x) {
        Console.WriteLine($"Constructing {name} with x {x}");

        this.name = name;
        this.x = x;
    }

    public static int Test(int i) {
        Console.WriteLine("Test success");
        return i * 2;
    }

    public void Echo(string str) {
        Console.WriteLine("Echoing: " + str);
    }
    public void Update(float dt) {
        x += dt;
        Console.WriteLine($"{name} updating by {dt} seconds and is now {x}");
        // Console.WriteLine($"{(float)dt}");
    }
}