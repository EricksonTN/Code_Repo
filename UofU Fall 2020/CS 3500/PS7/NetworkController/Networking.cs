using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace NetworkUtil
{
    /// <summary>
    /// An object created as a wrapper to pass around the user delegate and a TCPlistener
    /// <author>
    /// Written by Erickson Nguyen and Dylan Quach
    /// </author>
    /// </summary>
    public class ClientReceiver
    {
        public TcpListener listener;
        public Action<SocketState> ToCall;

        public ClientReceiver(TcpListener clientReceive, Action<SocketState> toCall)
        {
            listener = clientReceive;
            ToCall = toCall;
        }
    }

    /// <summary>
    /// A networking utility class designed to help servers and clients connect to each other and deliver data
    /// <author>
    /// Written by Daniel Kopta, Dylan Quach, and Erickson Nguyen
    /// </author>
    /// </summary>
    public static class Networking
    {
        /////////////////////////////////////////////////////////////////////////////////////////
        // Server-Side Code
        /////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Starts a TcpListener on the specified port and starts an event-loop to accept new clients.
        /// The event-loop is started with BeginAcceptSocket and uses AcceptNewClient as the callback.
        /// AcceptNewClient will continue the event-loop.
        /// </summary>
        /// <param name="toCall">The method to call when a new connection is made</param>
        /// <param name="port">The the port to listen on</param>
        public static TcpListener StartServer(Action<SocketState> toCall, int port)
        {
            ClientReceiver ClientReceive = new ClientReceiver(new TcpListener(IPAddress.Any, port), toCall);

            try
            {
                ClientReceive.listener.Start();
                ClientReceive.listener.BeginAcceptSocket(AcceptNewClient, ClientReceive);
            }
            catch (Exception)
            {
                ErrorHandler(toCall, "Could not start server. Please try again.");
            }

            return ClientReceive.listener;
        }

        /// <summary>
        /// To be used as the callback for accepting a new client that was initiated by StartServer, and 
        /// continues an event-loop to accept additional clients.
        ///
        /// Uses EndAcceptSocket to finalize the connection and create a new SocketState. The SocketState's
        /// OnNetworkAction should be set to the delegate that was passed to StartServer.
        /// Then invokes the OnNetworkAction delegate with the new SocketState so the user can take action. 
        /// 
        /// If anything goes wrong during the connection process (such as the server being stopped externally), 
        /// the OnNetworkAction delegate should be invoked with a new SocketState with its ErrorOccured flag set to true 
        /// and an appropriate message placed in its ErrorMessage field. The event-loop should not continue if
        /// an error occurs.
        ///
        /// If an error does not occur, after invoking OnNetworkAction with the new SocketState, an event-loop to accept 
        /// new clients should be continued by calling BeginAcceptSocket again with this method as the callback.
        /// </summary>
        /// <param name="ar">The object asynchronously passed via BeginAcceptSocket. It must contain a tuple with 
        /// 1) a delegate so the user can take action (a SocketState Action), and 2) the TcpListener</param>
        private static void AcceptNewClient(IAsyncResult ar)
        {
            ClientReceiver l = (ClientReceiver)ar.AsyncState;
            try
            {
                Socket s = (Socket)l.listener.EndAcceptSocket(ar);
                SocketState ss = new SocketState(l.ToCall, s);
                ss.OnNetworkAction(ss);
                l.listener.BeginAcceptSocket(AcceptNewClient, l);
            }

            catch (Exception)
            {
                ErrorHandler(l.ToCall, "Error whilst accepting client.");
            }
        }

        /// <summary>
        /// Stops the given TcpListener.
        /// </summary>
        public static void StopServer(TcpListener listener)
        {
            try
            {
                listener.Stop();
            }
            catch (Exception)
            {
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////
        // Client-Side Code
        /////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Begins the asynchronous process of connecting to a server via BeginConnect, 
        /// and using ConnectedCallback as the method to finalize the connection once it's made.
        /// 
        /// If anything goes wrong during the connection process, toCall should be invoked 
        /// with a new SocketState with its ErrorOccured flag set to true and an appropriate message 
        /// placed in its ErrorMessage field. Between this method and ConnectedCallback, toCall should 
        /// only be invoked once on error.
        ///
        /// This connection process should timeout and produce an error (as discussed above) 
        /// if a connection can't be established within 3 seconds of starting BeginConnect.
        /// 
        /// </summary>
        /// <param name="toCall">The action to take once the connection is open or an error occurs</param>
        /// <param name="hostName">The server to connect to</param>
        /// <param name="port">The port on which the server is listening</param>
        public static void ConnectToServer(Action<SocketState> toCall, string hostName, int port)
        {
            // TODO: This method is incomplete, but contains a starting point 
            //       for decoding a host address

            // Establish the remote endpoint for the socket.
            IPHostEntry ipHostInfo;
            IPAddress ipAddress = IPAddress.None;

            // Determine if the server address is a URL or an IP
            try
            {
                ipHostInfo = Dns.GetHostEntry(hostName);
                bool foundIPV4 = false;
                foreach (IPAddress addr in ipHostInfo.AddressList)
                    if (addr.AddressFamily != AddressFamily.InterNetworkV6)
                    {
                        foundIPV4 = true;
                        ipAddress = addr;
                        break;
                    }
                // Didn't find any IPV4 addresses
                if (!foundIPV4)
                {
                    // TODO: Indicate an error to the user, as specified in the documentation
                    ErrorHandler(toCall, "No valid IP Address is found.");
                }
            }
            catch (Exception)
            {
                // see if host name is a valid ipaddress
                try
                {
                    ipAddress = IPAddress.Parse(hostName);
                }
                catch (Exception)
                {
                    // TODO: Indicate an error to the user, as specified in the documentation
                    ErrorHandler(toCall, "Invalid Host Name.");
                }
            }

            // Create a TCP/IP socket.
            Socket socket = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            // This disables Nagle's algorithm (google if curious!)
            // Nagle's algorithm can cause problems for a latency-sensitive 
            // game like ours will be 
            socket.NoDelay = true;

            SocketState state = new SocketState(toCall, socket);

            try
            {
                //We save the beginConnect into an IAsyncResult so we can test it for a timeout
                IAsyncResult result = state.TheSocket.BeginConnect(ipAddress, port, ConnectedCallback, state);

                //If it takes longer than 3 seconds, then we create an error
                if (!result.AsyncWaitHandle.WaitOne(3000))
                {
                    ErrorHandler(toCall, "Server Connection Timed out");
                }

            }
            catch (Exception)
            {
                ErrorHandler(toCall, "Error connecting to Server.");
            }
        }

        /// <summary>
        /// To be used as the callback for finalizing a connection process that was initiated by ConnectToServer.
        ///
        /// Uses EndConnect to finalize the connection.
        /// 
        /// As stated in the ConnectToServer documentation, if an error occurs during the connection process,
        /// either this method or ConnectToServer (not both) should indicate the error appropriately.
        /// 
        /// If a connection is successfully established, invokes the toCall Action that was provided to ConnectToServer (above)
        /// with a new SocketState representing the new connection.
        /// 
        /// </summary>
        /// <param name="ar">The object asynchronously passed via BeginConnect</param>
        private static void ConnectedCallback(IAsyncResult ar)
        {
            SocketState state = (SocketState)ar.AsyncState;

            try
            {
                state.TheSocket.EndConnect(ar);
                SocketState newState = new SocketState(state.OnNetworkAction, state.TheSocket);
                state.OnNetworkAction(newState);
            }
            catch (Exception)
            {
                ErrorHandler(state.OnNetworkAction, "Couldn't connect to server. Please try again.");
            }
        }


        /////////////////////////////////////////////////////////////////////////////////////////
        // Server and Client Common Code
        /////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Begins the asynchronous process of receiving data via BeginReceive, using ReceiveCallback 
        /// as the callback to finalize the receive and store data once it has arrived.
        /// The object passed to ReceiveCallback via the AsyncResult should be the SocketState.
        /// 
        /// If anything goes wrong during the receive process, the SocketState's ErrorOccured flag should 
        /// be set to true, and an appropriate message placed in ErrorMessage, then the SocketState's
        /// OnNetworkAction should be invoked. Between this method and ReceiveCallback, OnNetworkAction should only be 
        /// invoked once on error.
        /// 
        /// </summary>
        /// <param name="state">The SocketState to begin receiving</param>
        public static void GetData(SocketState state)
        {
            try
            {
                state.TheSocket.BeginReceive(state.buffer, 0, SocketState.BufferSize, SocketFlags.None, ReceiveCallback, state);
            }
            catch (Exception)
            {
                ErrorHandler(state.OnNetworkAction, "Error receiving data.");
            }
        }

        /// <summary>
        /// To be used as the callback for finalizing a receive operation that was initiated by GetData.
        /// 
        /// Uses EndReceive to finalize the receive.
        ///
        /// As stated in the GetData documentation, if an error occurs during the receive process,
        /// either this method or GetData (not both) should indicate the error appropriately.
        /// 
        /// If data is successfully received:
        ///  (1) Read the characters as UTF8 and put them in the SocketState's unprocessed data buffer (its string builder).
        ///      This must be done in a thread-safe manner with respect to the SocketState methods that access or modify its 
        ///      string builder.
        ///  (2) Call the saved delegate (OnNetworkAction) allowing the user to deal with this data.
        /// </summary>
        /// <param name="ar"> 
        /// This contains the SocketState that is stored with the callback when the initial BeginReceive is called.
        /// </param>
        private static void ReceiveCallback(IAsyncResult ar)
        {
            //Initialize the socketstae that has been passed down
            SocketState s = (SocketState)ar.AsyncState;

            try
            {
                //End the Recieving Socket
                int numBytes = s.TheSocket.EndReceive(ar);
                //Begin the encoding process

                string recieved = Encoding.UTF8.GetString(s.buffer, 0, numBytes);
                s.data.Append(recieved);

                //Call the saved delegate
                s.OnNetworkAction(s);
            }
            catch (Exception)
            {
                ErrorHandler(s.OnNetworkAction, "Error Receiving Data.");
            }
        }

        /// <summary>
        /// Begin the asynchronous process of sending data via BeginSend, using SendCallback to finalize the send process.
        /// 
        /// If the socket is closed, does not attempt to send.
        /// 
        /// If a send fails for any reason, this method ensures that the Socket is closed before returning.
        /// </summary>
        /// <param name="socket">The socket on which to send the data</param>
        /// <param name="data">The string to send</param>
        /// <returns>True if the send process was started, false if an error occurs or the socket is already closed</returns>
        public static bool Send(Socket socket, string data)
        {
            if(!socket.Connected)
            {
                return false;
            }

            byte[] dataArr = Encoding.UTF8.GetBytes(data);
            try
            {
                socket.BeginSend(dataArr, 0, dataArr.Length, SocketFlags.None, SendCallback, socket);
            }
            catch(Exception)
            {
                socket.Close();
                return false;
            }

            return true;
        }

        /// <summary>
        /// To be used as the callback for finalizing a send operation that was initiated by Send.
        ///
        /// Uses EndSend to finalize the send.
        /// 
        /// This method must not throw, even if an error occured during the Send operation.
        /// </summary>
        /// <param name="ar">
        /// This is the Socket (not SocketState) that is stored with the callback when
        /// the initial BeginSend is called.
        /// </param>
        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                Socket s = (Socket)ar.AsyncState;
                s.EndSend(ar);
            }
            catch(Exception)
            {
                Socket s = (Socket)ar.AsyncState;
                s.Close();
            }
        }


        /// <summary>
        /// Begin the asynchronous process of sending data via BeginSend, using SendAndCloseCallback to finalize the send process.
        /// This variant closes the socket in the callback once complete. This is useful for HTTP servers.
        /// 
        /// If the socket is closed, does not attempt to send.
        /// 
        /// If a send fails for any reason, this method ensures that the Socket is closed before returning.
        /// </summary>
        /// <param name="socket">The socket on which to send the data</param>
        /// <param name="data">The string to send</param>
        /// <returns>True if the send process was started, false if an error occurs or the socket is already closed</returns>
        public static bool SendAndClose(Socket socket, string data)
        {
            if (!socket.Connected)
            {
                return false;
            }

            byte[] dataArr = Encoding.UTF8.GetBytes(data);
            try
            {
                socket.BeginSend(dataArr, 0, dataArr.Length, SocketFlags.None, SendAndCloseCallback, socket);
            }
            catch (Exception)
            {
                socket.Close();
                return false;
            }
            return true;
        }

        /// <summary>
        /// To be used as the callback for finalizing a send operation that was initiated by SendAndClose.
        ///
        /// Uses EndSend to finalize the send, then closes the socket.
        /// 
        /// This method must not throw, even if an error occured during the Send operation.
        /// 
        /// This method ensures that the socket is closed before returning.
        /// </summary>
        /// <param name="ar">
        /// This is the Socket (not SocketState) that is stored with the callback when
        /// the initial BeginSend is called.
        /// </param>
        private static void SendAndCloseCallback(IAsyncResult ar)
        {
            try
            {
                Socket s = (Socket)ar.AsyncState;
                s.EndSend(ar);
            }
            catch (Exception)
            {

            }
            finally
            {
                Socket s = (Socket)ar.AsyncState;
                s.Close();
            }
        }

        /// <summary>
        /// Takes in a user delegate and an error message. It creates a new socket and sets its error flag to true.
        /// Then it inputs the message provided into the socket before calling the user delegate
        /// </summary>
        /// <param name="toCall"></param>
        /// <param name="message"></param>
        private static void ErrorHandler (Action<SocketState> toCall, String message)
        {
            SocketState errorState = new SocketState(toCall, null);
            errorState.ErrorOccured = true;
            errorState.ErrorMessage = message;
            errorState.OnNetworkAction(errorState);
        }

    }
}

