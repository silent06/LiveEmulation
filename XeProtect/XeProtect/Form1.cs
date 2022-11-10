using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace XeProtect {
    public partial class Form1 : Form {
        public Form1() {
            InitializeComponent();
        }

        public struct TextSectionData {
            public UInt32 VirtualAddress;
            public UInt32 VirtualSize;
        }
        
        private static string BaseAddressString;
        private static string EntryPointString;
        private static uint BaseAddress;
        private static uint EntryPoint;
        private static string XexPath;
        private static byte[] XexBytes;
        private static int ArrayOffset;
        private static int ImportSize;

        private void Form1_Load(object sender, EventArgs e) {
            button1.DragEnter += new DragEventHandler(button1_DragEnter);
            button1.DragDrop += new DragEventHandler(button1_DragDrop);

            label4.ForeColor = Color.Green; // name
            label5.ForeColor = Color.Green; // entry
            label6.ForeColor = Color.Green; // base
            label8.ForeColor = Color.Green; // entry offset
            label10.ForeColor = Color.Green; // import size

            label4.Text = "";
            label5.Text = "";
            label6.Text = "";
            label8.Text = "";
            label10.Text = "";

            Height = 100;
        }

        private void button1_DragEnter(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy; // Okay
            else
                e.Effect = DragDropEffects.None; // Unknown data, ignore it

        }

        private void button1_DragDrop(object sender, DragEventArgs e) {
            string[] FileList = (string[])e.Data.GetData(DataFormats.FileDrop, false);

            string fileName = Path.GetFileName(FileList[0]);
            XexPath = FileList[0];

            if (fileName.Contains(".xex")) {
                button1.Enabled = false;

                XexBytes = File.ReadAllBytes(XexPath);

                ArrayOffset = FindArrayOffset();
                if (ArrayOffset == 0) {
                    MessageBox.Show("Obfuscation array not found.");
                    return;
                }

                LoadUIStringsFromReader(new BinaryReader(new MemoryStream(XexBytes)), XexPath.Contains("decrypted_LiveEmulation.xex"));

                ImportSize = GetImportSize();

                if (ImportSize == 0) {
                    MessageBox.Show("Couldn't determine import size. {0}", ImportSize.ToString());
                    return;
                }

                label4.Text = fileName;
                label5.Text = EntryPointString;
                label6.Text = BaseAddressString;
                label8.Text = "0x" + (BaseAddress + ArrayOffset + 0x1000).ToString("X");
                label10.Text = "0x" + ImportSize.ToString("X");

                richTextBox1.Clear();
                PrintToConsole("XeProtect Console v1.0\n", Color.White);

                Height = 530;
            } else {
                MessageBox.Show("Sorry, XeProtect only protects XeX files at the moment (Xbox Executables)");
            }
        }

        private void button1_Click(object sender, EventArgs e) {

        }

        private void label1_Click(object sender, EventArgs e) {

        }

        private void button3_Click(object sender, EventArgs e) {
            Reset();
        }

        private void Reset() {
            Height = 100;
            button1.Enabled = true;
            button2.Enabled = true;
            groupBox1.Enabled = true;
        }

        private void LoadUIStringsFromReader(BinaryReader reader, bool server) {
            if (server) {
                reader.ReadBytes(0x34); // padding

                byte[] ep = reader.ReadBytes(4);
                Array.Reverse(ep);

                EntryPointString = "0x" + BitConverter.ToUInt32(ep, 0).ToString("X");
                EntryPoint = BitConverter.ToUInt32(ep, 0);

                reader.ReadBytes(0x04); // padding

                byte[] ba = reader.ReadBytes(0x4);
                Array.Reverse(ba);

                BaseAddressString = "0x" + BitConverter.ToUInt32(ba, 0).ToString("X");
                BaseAddress = BitConverter.ToUInt32(ba, 0);
            } else {
                reader.ReadBytes(0x2C); // padding

                byte[] ep = reader.ReadBytes(4);
                Array.Reverse(ep);

                EntryPointString = "0x" + BitConverter.ToUInt32(ep, 0).ToString("X");
                EntryPoint = BitConverter.ToUInt32(ep, 0);

                reader.ReadBytes(0x04); // padding

                byte[] ba = reader.ReadBytes(0x4);
                Array.Reverse(ba);

                BaseAddressString = "0x" + BitConverter.ToUInt32(ba, 0).ToString("X");
                BaseAddress = BitConverter.ToUInt32(ba, 0);
            }
        }

        /*private static int SearchByteByByte(byte[] bytes, byte[] pattern) {
            for (int i = 0; i < bytes.Length; i++) {
                if (bytes.Length - i < pattern.Length)
                    return 0;

                if (pattern[0] != bytes[i])
                    continue;

                for (int j = 0; j < pattern.Length; j++) {
                    if (bytes[i + j] != pattern[j])
                        break;

                    if (j == pattern.Length - 1)
                        return i;
                }
            }

            return 0;
        }*/

        public static int SearchByteByByte(byte[] bytes, byte[] pattern)
        {
            for (int i = 0; i < bytes.Length; i++)
            {
                if (bytes.Length - i < pattern.Length)
                {
                    return 0;
                }
                if (pattern[0] != bytes[i])
                {
                    continue;
                }
                for (int j = 0; j < pattern.Length && bytes[i + j] == pattern[j]; j++)
                {
                    if (j == pattern.Length - 1)
                    {
                        return i;
                    }
                }
            }
            return 0;
        }


        private int FindArrayOffset() {
            return SearchByteByByte(XexBytes, new byte[] { 0x53, 0x75, 0x70, 0x65, 0x72, 0x5f, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65 });
        }
        // 0x53, 0x75, 0x70, 0x65, 0x72, 0x5f, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65
        private int FindSections() {
            return SearchByteByByte(XexBytes, new byte[] { 0x2e, 0x72, 0x64, 0x61, 0x74, 0x61 });
        }

        private TextSectionData GetTextSection() {
            TextSectionData textSection = new TextSectionData();
            int dotText = SearchByteByByte(XexBytes, new byte[] { 0x2e, 0x74, 0x65, 0x78, 0x74 });
            if (dotText != 0) {
                BinaryReader reader = new BinaryReader(new MemoryStream(XexBytes.Skip(dotText).ToArray()));

                byte[] name = reader.ReadBytes(8);
                uint VirtualSize = reader.ReadUInt32();
                uint VirtualAddress = reader.ReadUInt32();
                uint SizeOfRawData = reader.ReadUInt32();
                uint PointerToRawData = reader.ReadUInt32();
                uint PointerToRelocations = reader.ReadUInt32();
                uint PointerToLinenumbers = reader.ReadUInt32();
                ushort NumberOfRelocations = reader.ReadUInt16();
                ushort NumberOfLinenumbers = reader.ReadUInt16();
                uint Characteristics = reader.ReadUInt32();

                textSection.VirtualSize = VirtualSize;
                textSection.VirtualAddress = VirtualAddress;

                reader.Close();
            }

            return textSection;
        }

        private int GetImportSize() {
            int xamXex = SearchByteByByte(XexBytes, new byte[] { 0x02, 0x78, 0x61, 0x6D, 0x2E, 0x78, 0x65, 0x78 });
            //0x02, 0x78, 0x61, 0x6D, 0x2E, 0x78, 0x65, 0x78
            //if (xamXex == 0) {
                xamXex += 0x45;

                BinaryReader reader = new BinaryReader(new MemoryStream(XexBytes));
                byte[] padding = reader.ReadBytes(xamXex);

                byte[] firstImport = reader.ReadBytes(4);
                Array.Reverse(firstImport);

                uint fImport = BitConverter.ToUInt32(firstImport, 0);

                byte[] importPadding = new byte[] {
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x45, 0xDC, 0x17, 0xE0,
                    0x20, 0x51, 0x87, 0x00, 0x20, 0x07, 0x45, 0x00,
                    0x00, 0x01, 0x00
                };//51 87 0x53, 0x08,

                int iPad = SearchByteByByte(XexBytes, importPadding);
                if (iPad != 0) {
                    iPad += 0x24;

                    int scanner = iPad;
                    while (true) {
                        if (XexBytes[scanner] != 0x91)
                            break;

                        scanner += 4;
                    }

                    scanner -= 4;

                    byte[] temp = new byte[] { XexBytes[scanner], XexBytes[scanner + 1], XexBytes[scanner + 2], XexBytes[scanner + 3] };
                    Array.Reverse(temp);

                    uint lastAddress = BitConverter.ToUInt32(temp, 0) + 0xC;
                    return (int)(lastAddress - fImport);
                }
            //}

            return 0;
        }

        private void button2_Click(object sender, EventArgs e) {
            button2.Enabled = false;
            groupBox1.Enabled = false;

            byte[] xorKeys = new byte[20];
            uint blockDataStartAddress = 0;
            uint blockDataEndAddress = 0;
            byte blockDataRoundCount = 0;

            byte[] idaCrashBytes = new byte[10];
            uint idaCrashStartAddress = 0;

            byte importXorKey = (byte)new Random().Next(0xFF);
            byte[] importData = new byte[1000];
            int importArraySize = 0;
            uint importStartAddress = 0;
            uint importEndAddress = 0;

            byte[] gprlBytes = new byte[0x50];
            byte[] gprlEncBytes = new byte[0x50];
            uint gprlStart = 0;
            uint gprlEnd = 0;
            uint gprlStartOffset = 0;
            uint gprlEndOffset = 0;
            bool hasGprl = false;

            if (checkBox1.Checked) {
                int xamXex = SearchByteByByte(XexBytes, new byte[] { 0x02, 0x78, 0x61, 0x6D, 0x2E, 0x78, 0x65, 0x78 });
                if (xamXex != 0) {
                    xamXex += 0x41;

                    importStartAddress = (uint)(xamXex);

                    while (true) {
                        if (XexBytes[xamXex] == 0x4D && XexBytes[xamXex + 1] == 0x5A)
                            break;
                        xamXex++;
                    }

                    importEndAddress = (uint)(xamXex);

                    for (uint i = importStartAddress; i < importEndAddress; i++) {
                        importData[importArraySize] = XexBytes[i];
                        importData[importArraySize] ^= importXorKey;
                        XexBytes[i] = 0;
                        importArraySize++;
                    }
                }
            }

            if (checkBox2.Checked) {
                uint entryPointOffset = EntryPoint - BaseAddress + 0x1000;
                for (int i = 0; i < 16; i += 4) {
                    if (XexBytes[entryPointOffset + i] == 0x48 || XexBytes[entryPointOffset + i] == 0x4B) {
                        hasGprl = true;
                        PrintToConsole("Found GPLR\n", Color.HotPink);

                        byte[] tmp = new byte[] { XexBytes[(int)(entryPointOffset + i + 2)], XexBytes[(int)(entryPointOffset + i + 3)] };

                        Array.Reverse(tmp);

                        short offset = BitConverter.ToInt16(new byte[2] { tmp[0], tmp[1] }, 0);
                        uint relativeGplr = (uint)(entryPointOffset + i + offset - 0x10001);

                        uint relativeSearchTop = relativeGplr;
                        while (XexBytes[relativeSearchTop] != 0) {
                            relativeSearchTop--;
                        }
                        relativeSearchTop++;

                        uint relativeSearchEnd = relativeGplr;
                        while (XexBytes[relativeSearchEnd] != 0x4E && XexBytes[relativeSearchEnd + 1] != 0x80) {
                            relativeSearchEnd++;
                        }

                        gprlStart = BaseAddress + relativeSearchTop - 0x1000;
                        gprlEnd = BaseAddress + relativeSearchEnd - 0x1000;
                        gprlStartOffset = relativeSearchTop;
                        gprlEndOffset = relativeSearchEnd;

                        Array.Copy(XexBytes, relativeSearchTop, gprlBytes, 0, 0x50);

                        PrintToConsole(string.Format("GPRL Start: {0:X}, End: {1:X}\n", gprlStart, gprlEnd), Color.HotPink);
                        break;
                    }
                }
            }

            TextSectionData textSection = GetTextSection();
            if (textSection.VirtualAddress != 0) {
                PrintToConsole("Found .text section\n", Color.HotPink);
                PrintToConsole(string.Format("> Virtual Address: 0x{0:X}\n", textSection.VirtualAddress), Color.White);
                PrintToConsole(string.Format("> Virtual Size: 0x{0:X}\n", textSection.VirtualSize), Color.White);
                PrintToConsole(string.Format("> Start: 0x{0:X}\n", BaseAddress + textSection.VirtualAddress), Color.White);

                uint startOffset = textSection.VirtualAddress;
                uint endOffset = (uint)((startOffset + textSection.VirtualSize) - ImportSize) - 8;
                uint size = endOffset - startOffset;

                blockDataStartAddress = BaseAddress + startOffset;
                blockDataEndAddress = BaseAddress + endOffset;

                int rounds = checkBox3.Checked ? new Random().Next(4, 20) : 3;
                blockDataRoundCount = (byte)rounds;

                Random keyShit = new Random();
                
                /*Needs work*/
                /*for (int j = 0; j < rounds; j++) {
                    xorKeys[j] = (byte)keyShit.Next(0xFF);

                    for (uint i = 0; i < (size / 2); i++) {
                        byte top = XexBytes[(startOffset + 0x1000) + i];
                        byte bottom = XexBytes[(endOffset + 0x1000) - i];

                        XexBytes[(startOffset + 0x1000) + i] = bottom;
                        XexBytes[(endOffset + 0x1000) - i] = top;

                        XexBytes[(startOffset + 0x1000) + i] ^= xorKeys[j];
                        XexBytes[(endOffset + 0x1000) - i] ^= xorKeys[j];
                    }
                }*/

                if (checkBox4.Checked) {
                    for (uint i = startOffset; i < endOffset; i += 4) {
                        Array.Reverse(XexBytes, (int)(i + 0x1000), 4);
                    }
                }

                if (checkBox5.Checked) {
                    int xamXex = SearchByteByByte(XexBytes, new byte[] { 0x02, 0x78, 0x61, 0x6D, 0x2E, 0x78, 0x65, 0x78 });
                    if (xamXex != 0) {
                        xamXex += 19;
                        idaCrashStartAddress = (uint)(xamXex);

                        for (int i = 0; i < 10; i++) {
                            idaCrashBytes[i] = XexBytes[xamXex + i];
                            XexBytes[xamXex + i] = 0xCC;
                        }

                        PrintToConsole("Crash IDA patch was successful\n", Color.HotPink);
                    }
                }

                if (checkBox6.Checked) {
                    int pdb = SearchByteByByte(XexBytes, new byte[] { 0x2E, 0x70, 0x64, 0x62 });
                    if (pdb != 0) {
                        pdb += 3;

                        int end = pdb;
                        int start = end;

                        while (XexBytes[start] != 0) {
                            start--;
                        }

                        for (int i = start; i <= end; i++) {
                            XexBytes[i] = 0;
                        }

                        PrintToConsole("Strip debug symbols patch was successful\n", Color.HotPink);
                    }
                }

                // copy back the gprl
                if (hasGprl) {
                    Array.Copy(XexBytes, gprlStartOffset, gprlEncBytes, 0, 0x50);
                    Array.Copy(gprlBytes, 0, XexBytes, gprlStartOffset, 0x50);
                }

                PrintToConsole(string.Format("Encrypted with {0} rounds\n", rounds), Color.HotPink);
            } else {
                MessageBox.Show("Failed to find text section. Oopsie Doopsie!");
                Reset();
                return;
            }

            byte[] mockArray = new byte[10000];
            EndianWriter writer = new EndianWriter(new MemoryStream(mockArray), EndianStyle.BigEndian);

            // header
            writer.Write(checkBox1.Checked); // m_importEncryption
            writer.Write(checkBox4.Checked); // m_endianSwap
            writer.Write(checkBox5.Checked); // m_idaProtection

            // if import protection
            if (checkBox1.Checked) {
                writer.Write(importStartAddress);
                writer.Write(importEndAddress);
                writer.Write(importArraySize);
                writer.Write(importXorKey);
                writer.Write(importData);


            }

            // if crash ida
            if (checkBox5.Checked) {
                writer.Seek(2017, SeekOrigin.Begin);
                writer.Write(idaCrashStartAddress);
                writer.Write(idaCrashBytes);
            }

            // blocks
            writer.Seek(2031, SeekOrigin.Begin);
            writer.Write(hasGprl);
            writer.Write(gprlEncBytes);
            writer.Write(gprlStart);
            writer.Write(gprlEnd);
            writer.Write(blockDataRoundCount);
            writer.Write(xorKeys);
            writer.Write(blockDataStartAddress);
            writer.Write(blockDataEndAddress);

            writer.Close();

            Array.Copy(mockArray, 0, XexBytes, ArrayOffset, mockArray.Length);

            File.Create("mock.bin").Write(mockArray, 0, mockArray.Length);

            using (StreamWriter sw = File.CreateText(Path.GetDirectoryName(XexPath) + "\\Secure-" + Path.GetFileName(XexPath))) {
                sw.BaseStream.Write(XexBytes, 0, XexBytes.Length);
            }

            PrintToConsole(string.Format("Output: Secure-{0}", Path.GetFileName(XexPath)), Color.Goldenrod);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e) {

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e) {

        }

        private void groupBox2_Enter(object sender, EventArgs e) {

        }

        private void PrintToConsole(string message, Color color) {
            richTextBox1.SelectionColor = color;
            richTextBox1.AppendText(message);
            richTextBox1.SelectionColor = richTextBox1.ForeColor;
        }
    }
}
