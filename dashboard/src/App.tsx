import { useWebSocket } from './hooks/useWebSocket';
import { MessageTable } from './components/MessageTable';
import { StatsPanel } from './components/StatsPanel';

function App() {
  const envUrl = import.meta.env.VITE_WS_URL as string | undefined;
  const proto = typeof window !== 'undefined' && window.location.protocol === 'https:' ? 'wss' : 'ws';
  const host = typeof window !== 'undefined' ? window.location.hostname : 'localhost';
  const defaultUrl = `${proto}://${host}:8080`;
  // If env points to a Docker-only hostname (e.g., cantrol-bridge), prefer browser-derived host
  const wsUrl = envUrl && /cantrol-bridge|^bridge$/i.test(envUrl) ? defaultUrl : (envUrl ?? defaultUrl);
  const { frames, totalCount, lastTs } = useWebSocket(wsUrl);

  return (
    <div className="min-h-screen px-8 py-6">
      <header className="mb-6">
        <h1 className="text-3xl font-medium text-gray-200">
          CANtrol
        </h1>
        <p className="text-md text-gray-400">
          Live CAN bus inspection
        </p>
      </header>

      <StatsPanel frames={frames} totalCount={totalCount} lastTs={lastTs} />

      <div className="mt-6">
        <MessageTable frames={frames} />
      </div>
    </div>
  );
}

export default App;
