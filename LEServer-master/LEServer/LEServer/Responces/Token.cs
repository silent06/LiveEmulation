using System;
using Security;
using LE;
using System.Text;
using System.Collections.Generic;

namespace LE.Responces {
   class Token {

        public static void ProcessVerifyTkn(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj, ref TOKEN_STRUCT TokenObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string Token = new string(io.reader.ReadChars(0xC));
            string Ip = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
            
            byte[] Resp = new byte[0xC];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                Data.Write((int)PACKET_STATUS.SUCCESS);

                if (MySql.isValid(ref TokenObj, Token) && !MySql.isBlackListed(Token)) {
                    if (!MySql.isUsed(Token))
                        Data.Write((int)TOKEN_STATUS.CANREDEEM);
                    else
                        Data.Write((int)TOKEN_STATUS.ALREADYREDEEMED);
                } else
                    Data.Write((int)TOKEN_STATUS.INVALIDTOKEN);
            } else {
                Data.Write((int)PACKET_STATUS.ERROR);
                Data.Write((int)TOKEN_STATUS.ERROR);
            }
            Data.Write(TokenObj.Days);
            io.writer.Write(Resp);
            MySql.SaveClient(ClientObj, SessionToken);
        }

        public static void ProcessRedeemTkn(ClientHandler.ioData io, ref CLIENT_STRUCT ClientObj, ref TOKEN_STRUCT TokenObj) {
            string SessionToken = Utilities.BytesToString(io.reader.ReadBytes(0x10));
            string Token = new string(io.reader.ReadChars(0xC));
            string Ip = io.ipaddr.Address.ToString().Split(new char[] { ':' })[0];
            
            string Status = "Error";
            string TokenMessage = "LiveEmulation - Redeem Error!";
            byte[] MessageBuffer = new byte[0x30];
            byte[] Resp = new byte[MessageBuffer.Length + 0x8];
            EndianWriter Data = new EndianIO(Resp, EndianStyle.BigEndian).Writer;

            bool ClientFound = MySql.GetClient(ref ClientObj, SessionToken);
            if (ClientFound) {
                Data.Write((int)PACKET_STATUS.SUCCESS);
                if (MySql.isValid(ref TokenObj, Token) && !MySql.isBlackListed(Token)) {
                    if (!MySql.isUsed(Token)) {
                        if (!MySql.Redeem(TokenObj, ref ClientObj)) {
                            Data.Write((int)TOKEN_STATUS.ERROR);
                            TokenMessage = "LEmulation - Failed to redeem!";
                        } else {
                            Data.Write((int)TOKEN_STATUS.CANREDEEM);
                            Status = "Successfully redeeemed!";
                            TokenMessage = string.Format("LEmulation - Successfully redeemed: {0} days!", TokenObj.Days);
                        }
                    } else {
                        Data.Write((int)TOKEN_STATUS.ALREADYREDEEMED);
                        Status = "Already used!";
                        TokenMessage = "LEmulation - Token has already been redeemed!";
                    }
                } else {
                    Data.Write((int)TOKEN_STATUS.INVALIDTOKEN);
                    Status = "Invalid token!";
                    TokenMessage = "LEmulation - Token not valid!";
                }
            } else {
                Data.Write((int)PACKET_STATUS.SUCCESS);
                Data.Write((int)TOKEN_STATUS.ERROR);
                Status = "Unknown token error!";
            }

            List<Log.PrintQueue> RedeemTokenId = Log.GetQueue();
            Log.Add(RedeemTokenId, ConsoleColor.DarkMagenta, "|TOKEN REDEEM|", null);
            Log.Add(RedeemTokenId, ConsoleColor.Magenta, "CPUKey:", $"{ClientObj.cpukey}");
            Log.Add(RedeemTokenId, ConsoleColor.Magenta, "Token:", $"{Token}");
            Log.Add(RedeemTokenId, ConsoleColor.Yellow, "Status:", $"{Status}");
            Log.Print(RedeemTokenId);

            Buffer.BlockCopy(Encoding.ASCII.GetBytes(TokenMessage), 0, MessageBuffer, 0, TokenMessage.Length);
            Data.Write(MessageBuffer);
            io.writer.Write(Resp);
        }
    }
}