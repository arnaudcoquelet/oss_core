// Library: OSS Software Solutions Application Programmer Interface
// Author: Joegen E. Baclor - mailto:joegen@ossapp.com
//
// Copyright (c) OSS Software Solutions
//
// Permission is hereby granted, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, execute, and to prepare
// derivative works of the Software, all subject to the
// "GNU Lesser General Public License (LGPL)".
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

//
// WebSocket++ has the following license :
//

/*
 * Copyright (c) 2011, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef OSS_SIPWEBSOCKETLISTENER_H_INCLUDED
#define	OSS_SIPWEBSOCKETLISTENER_H_INCLUDED


#include "OSS/SIP/SIPWebSocketConnection.h"
#include "OSS/SIP/SIPWebSocketConnectionManager.h"
#include "OSS/SIP/SIPListener.h"
#include "OSS/Thread.h"


namespace OSS {
namespace SIP {


class SIPFSMDispatch;

class SIPWebSocketListener : public OSS::SIP::SIPListener
{
protected:
  class ServerHandler : public websocketpp::server::handler
  {
  public:
    ServerHandler(SIPWebSocketListener& listener);
    void on_message(websocketpp::server::connection_ptr pConnection, websocketpp::server::handler::message_ptr pMsg);
    void on_handshake_init(websocketpp::server::connection_ptr pConnection);
    SIPWebSocketListener& _listener;
    friend class SIPWebSocketListener;
  };

  class ClientHandler : public websocketpp::client::handler
  {
  public:
    ClientHandler(SIPWebSocketListener& listener);
    void on_message(websocketpp::client::connection_ptr pConnection, websocketpp::client::handler::message_ptr pMsg);
    SIPWebSocketListener& _listener;
    friend class SIPWebSocketListener;
  };

public:

  SIPWebSocketListener(
    SIPTransportService* pTransportService,
    const std::string& address,
    const std::string& port,
    SIPWebSocketConnectionManager& connectionManager);

  ~SIPWebSocketListener();

  virtual void run();
    /// Run the server's io_service loop.

  virtual void handleAccept(const boost::system::error_code& e, OSS_HANDLE userData = 0);
    /// Handle completion of an asynchronous accept operation.

  virtual void handleStop();
    /// Handle a request to stop the server.

  virtual void connect(const std::string& address, const std::string& port);
    /// Connect to the specified address and port

  virtual void handleConnect(const boost::system::error_code& e, boost::asio::ip::tcp::resolver::iterator endPointIter);
    /// Handle completion of the connect operation.

  
protected:
  void internal_run();

  void on_message(websocketpp::server::connection_ptr pConnection, websocketpp::server::handler::message_ptr pMsg);
  void on_message(websocketpp::client::connection_ptr pConnection, websocketpp::client::handler::message_ptr pMsg);
  websocketpp::server::handler::ptr _pServerHandler;
  websocketpp::server* _pServerEndPoint;
  websocketpp::client::handler::ptr _pClientHandler;
  websocketpp::client* _pClientEndPoint;
  SIPWebSocketConnectionManager& _connectionManager;
  boost::asio::ip::tcp::resolver* _pResolver;
  boost::thread* _pServerThread;
  boost::thread* _pClientThread;
    /// The resolver service;
  friend class ServerHandler;
  friend class ClientHandler;
};


//
// Inlines
//

inline SIPWebSocketListener::ClientHandler::ClientHandler(SIPWebSocketListener& listener) :
  _listener(listener)
{
}

inline void SIPWebSocketListener::ClientHandler::on_message(websocketpp::client::connection_ptr pConnection, websocketpp::client::handler::message_ptr pMsg)
{
  _listener.on_message(pConnection, pMsg);
}

inline SIPWebSocketListener::ServerHandler::ServerHandler(SIPWebSocketListener& listener) :
  _listener(listener)
{
}

inline void SIPWebSocketListener::ServerHandler::on_message(websocketpp::server::connection_ptr pConnection, websocketpp::server::handler::message_ptr pMsg)
{
  _listener.on_message(pConnection, pMsg);
}

inline void SIPWebSocketListener::ServerHandler::on_handshake_init(websocketpp::server::connection_ptr pConnection)
{
  boost::system::error_code ec;
  _listener.handleAccept(ec, &pConnection);
}

} } // OSS::SIP

#endif	/// OSS_SIPWEBSOCKETLISTENER_H_INCLUDED

