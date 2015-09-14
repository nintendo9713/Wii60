using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using WiimoteLib;
using System.IO.Ports;

#region CollectWiimotes Class
// Contains Dictionary to map Guids to a Class Instance
// Contains our Wiimote Collection to manage all of our connected wiimotes

public partial class CollectWiimotes
{
    // Dictionary holds Guids and class instances
    // Allows us to easily access an individual wiimote
    private static Dictionary<Guid, WiimoteInfo> mWiimoteMap = new Dictionary<Guid, WiimoteInfo>();
    // WiiMoteCollection is a class from the DLL that stores all 
    // connected wiimotes
    private static WiimoteCollection mWC = new WiimoteCollection();

    // Standard Get/Set methods for our Dictionary/Collection
    public static Dictionary<Guid, WiimoteInfo> wmMap
    {
        get { return mWiimoteMap; }
        set { mWiimoteMap = value; }
    }

    public static WiimoteCollection WiiCollect
    {
        get { return mWC; }
        set { mWC = value; }
    }
}
#endregion

#region WiimoteInfo Class
// Contains procedures to read from and write to Wiimotes
public partial class WiimoteInfo
{
    private Wiimote mWiimote;

    public int WiimoteLED;

    public SerialPort sp = new SerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
    

    byte[] buff = new byte[23];
    int modCount = 1;
    double temp;
    byte xAccel, yAccel, zAccel, xNun, yNun, zNun, xJoy, yJoy, ABut, BBut, OneBut, TwoBut, UpBut, DownBut, LeftBut, RightBut, PlusBut, MinusBut, HomeBut, CBut, ZBut, xIR, yIR;


    // semaphore used to prevent race condition in console display
    static private Semaphore updateSemaphore = new Semaphore(1, 1);

    //Default Constructor
    public WiimoteInfo()
    {
    }

    // Assignment constructor
    // Is passed a Wiimote object and assigns it to the instance in the WiimoteInfo class
    public WiimoteInfo(Wiimote wm)
        : this()
    {
        mWiimote = wm;
        wm.SetLEDs(true, false, false, false);//recentlly added for GTA video
        sp.Open();
    }

    // Used for updating the wiimote itself
    // This is where you put output statements to see changes in the wiimote state
    public void UpdateState(WiimoteChangedEventArgs args)
    {

        WiimoteState ws = args.WiimoteState;
        updateSemaphore.WaitOne();
        Console.SetCursorPosition(0, this.WiimoteLED);

        if ((temp = ws.AccelState.Values.X) < 1 && temp > -1)
            buff[0] = xAccel = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.AccelState.Values.Y) < 1 && temp > -1)
            buff[1] = yAccel = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.AccelState.Values.Z) < 1 && temp > -1)
            buff[2] = zAccel = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.NunchukState.AccelState.Values.X) < 1 && temp > -1)
            buff[3] = xNun = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.NunchukState.AccelState.Values.Y) < 1 && temp > -1)
            buff[4] = yNun = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.NunchukState.AccelState.Values.Z) < 1 && temp > -1)
            buff[5] = zNun = Convert.ToByte(temp * 128 + 127);
        if ((temp = ws.NunchukState.Joystick.X) < 0.5 && temp > -0.5)
            buff[6] = xJoy = Convert.ToByte(temp * 256 + 127);
        if ((temp = ws.NunchukState.Joystick.Y) < 0.5 && temp > -0.5)
            buff[7] = yJoy = Convert.ToByte(temp * 256 + 127);
        buff[8] = ABut = Convert.ToByte(ws.ButtonState.A);
        buff[9] = BBut = Convert.ToByte(ws.ButtonState.B);
        buff[10] = OneBut = Convert.ToByte(ws.ButtonState.One);
        buff[11] = TwoBut = Convert.ToByte(ws.ButtonState.Two);
        buff[12] = UpBut = Convert.ToByte(ws.ButtonState.Up);
        buff[13] = DownBut = Convert.ToByte(ws.ButtonState.Down);
        buff[14] = LeftBut = Convert.ToByte(ws.ButtonState.Left);
        buff[15] = RightBut = Convert.ToByte(ws.ButtonState.Right);
        buff[16] = PlusBut = Convert.ToByte(ws.ButtonState.Plus);
        buff[17] = MinusBut = Convert.ToByte(ws.ButtonState.Minus);
        buff[18] = HomeBut = Convert.ToByte(ws.ButtonState.Home);
        buff[19] = CBut = Convert.ToByte(ws.NunchukState.C);
        buff[20] = ZBut = Convert.ToByte(ws.NunchukState.Z);

        if (ws.IRState.IRSensors[0].Found && ws.IRState.IRSensors[1].Found)
        {
            buff[21] = xIR = Convert.ToByte(ws.IRState.IRSensors[0].Position.X*255);
            buff[22] = yIR = Convert.ToByte(ws.IRState.IRSensors[0].Position.Y*255);
       }
        Console.Clear();
        
        Console.WriteLine("\n\n\n\n\n" +
            xAccel + "     " + yAccel + "     " + zAccel + "\n" +
            xNun + "     " + yNun + "     " + zNun + "\n " +
            xJoy + "     " + yJoy + "     " + "\n" +
            ABut + BBut + OneBut + TwoBut + UpBut + DownBut + LeftBut + RightBut + PlusBut + MinusBut + HomeBut + CBut + ZBut + "\n" +
            xIR + "     " + yIR + "\n");
        
        if (modCount%10 == 0)
            sp.Write(buff, 0, 23);

        modCount++;
        if (modCount == 32000)
            modCount = 0;

        updateSemaphore.Release();

    }

    // Used for updating the extensions (if any are plugged in)
    public void UpdateExtension(WiimoteExtensionChangedEventArgs args)
    {
        updateSemaphore.WaitOne();
        Console.SetCursorPosition(40, this.WiimoteLED);
        Console.WriteLine("                     ");
        Console.SetCursorPosition(40, this.WiimoteLED);
        Console.WriteLine(args.ExtensionType.ToString());
        updateSemaphore.Release();

    }

    // get/set methods for the class
    public Wiimote Wiimote
    {
        get { return mWiimote; }
        set { mWiimote = value; }
    }
}
#endregion

#region HelloWorld Console Application
namespace WiiRemote_HelloWorld_Console_Application
{
    class Program
    {

        static void Main(string[] args)
        {
            int wiimoteIndex = 0;  //Used to identify the wiimotes.  

            // Try and find all wiimotes.  Throw an error if we can't find any.
            try
            {
                CollectWiimotes.WiiCollect.FindAllWiimotes();

            }
            catch (WiimoteNotFoundException ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Wiimote not found");
            }
            catch (WiimoteException ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Wiimote Error");

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.WriteLine("Unknown Error In Acquiring Wiimote");
            }

            // Run through each wiimote we found in the HID list and:
            //   Instantiate a new Class
            //   Put its ID into the dictionary
            //   Connects Wiimote
            //   Associate the LEDE number with the instance
            //   Changes LED to indicate unit number

            foreach (Wiimote wm in CollectWiimotes.WiiCollect)
            {
                Console.Clear();

                // Create a new class instance for each wiimote we find in HID list
                WiimoteInfo wi = new WiimoteInfo(wm);

                // Place our wiimote's ID into the dictionary.
                CollectWiimotes.wmMap[wm.ID] = wi;

                wiimoteIndex++;

                // Set  the LED count
                wi.WiimoteLED = wiimoteIndex;

                // Connect it and tell it which functions to call when the wiimote updates state
                wm.WiimoteChanged += wm_WiimoteChanged;     // add functor
                wm.WiimoteExtensionChanged += wm_WiimoteExtensionChanged;
                wm.Connect();

                // Set the acquisition rate for the wiimote
                if (wm.WiimoteState.ExtensionType != ExtensionType.BalanceBoard)
                    wm.SetReportType(InputReport.IRExtensionAccel, WiimoteLib.IRSensitivity.Maximum, true);

                // Set LEDs so we know it is connected
                wm.SetLEDs(wi.WiimoteLED);

                Console.SetCursorPosition(0, wi.WiimoteLED);
                Console.WriteLine("Wiimote " + wi.WiimoteLED.ToString() + " Connected!");
                Console.SetCursorPosition(0, wi.WiimoteLED + 1);
                Console.WriteLine("Press escape to exit");
            }

            // Hang around and display the battery value (in UpdateState)
            ConsoleKeyInfo info = Console.ReadKey();
            while (info.Key != ConsoleKey.Escape)
            {
                info = Console.ReadKey();
            }
            Console.Clear();

        }

        // mathod that gets called any time the wiimote changes state
        private static void wm_WiimoteChanged(object sender, WiimoteChangedEventArgs e)
        {
            WiimoteInfo wi = CollectWiimotes.wmMap[((Wiimote)sender).ID];
            wi.UpdateState(e);
        }

        // Invoked when extensions are attached
        private static void wm_WiimoteExtensionChanged(object sender, WiimoteExtensionChangedEventArgs e)
        {
            // find the control for this Wiimote
            WiimoteInfo wi = CollectWiimotes.wmMap[((Wiimote)sender).ID];
            wi.UpdateExtension(e);

            if (e.Inserted)
                ((Wiimote)sender).SetReportType(InputReport.IRExtensionAccel, true);
            else
                ((Wiimote)sender).SetReportType(InputReport.IRAccel, true);
        }
    }
}
#endregion
