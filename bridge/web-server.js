const net = require('net');
const WebSocket = require('ws');

const WS_PORT = 8080;
const TCP_PORT = 9000;

// WebSocket server for dashboard
const wss = new WebSocket.Server({ port: WS_PORT });
console.log(`WebSocket server running on ws://0.0.0.0:${WS_PORT}`);

function broadcast(frame) {
  const msg = JSON.stringify(frame);
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(msg);
    }
  });
}

// TCP server to receive frames from simulator
const tcpServer = net.createServer(socket => {
  console.log('Simulator connected via TCP');

  socket.on('data', data => {
    const lines = data.toString().split('\n');
    for (const line of lines) {
      if (!line.trim()) continue;
      try {
        const frame = JSON.parse(line);
        broadcast(frame); 
      } catch (err) {
        console.error('Invalid JSON from simulator:', line);
      }
    }
  });

  socket.on('close', () => console.log('Simulator disconnected'));
});

tcpServer.listen(TCP_PORT, () => console.log(`TCP server running on port ${TCP_PORT}`));
