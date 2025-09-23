import type { CANFrame } from '../hooks/useWebSocket';

type Props = {
  frames: CANFrame[];
  totalCount?: number;
  lastTs?: number | null;
  bitrate?: number; // in bits per second, for bus load approximation
};

export const StatsPanel = ({ frames, totalCount = 0, lastTs = null, bitrate = 500_000 }: Props) => {
  // Frames per node (current window)
  const perNode: Record<number, number> = {};
  // Frames per CAN ID (current window)
  const perId: Record<number, number> = {};
  frames.forEach(f => {
    perNode[f.node] = (perNode[f.node] || 0) + 1;
    perId[f.id] = (perId[f.id] || 0) + 1;
  });

  // FPS over last 1 second window
  const now = Date.now();
  const oneSecAgo = now - 1000;
  const fps = frames.reduce((acc, f) => acc + (f.ts >= oneSecAgo ? 1 : 0), 0);

  // Approx bus load: payload bits per second divided by bitrate
  const payloadBitsLastSec = frames.reduce((acc, f) => acc + (f.ts >= oneSecAgo ? f.dlc * 8 : 0), 0);
  const busLoad = bitrate > 0 ? Math.min(1, payloadBitsLastSec / bitrate) : 0;

  return (
    <div className="bg-[#252526] rounded-md px-4 py-3">
      <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-6 gap-6">
        <Stat label="Total Frames" value={totalCount.toLocaleString()} />
        <Stat label="FPS" value={fps.toString()} />
        <Stat label="Last Frame" value={lastTs ? new Date(lastTs).toLocaleTimeString() : '—'} />
        <Stat label="Bus Load (approx)" value={`${(busLoad * 100).toFixed(1)}%`} />
        <Stat label="Unique IDs (window)" value={Object.keys(perId).length.toString()} />
        <Stat label="Nodes Active (window)" value={Object.keys(perNode).length.toString()} />
      </div>

      <div className="mt-6 grid grid-cols-1 md:grid-cols-2 gap-6">
        <Panel title="Frames per Node">
          <ul className="text-sm text-gray-300">
            {Object.entries(perNode).sort((a,b) => Number(b[1]) - Number(a[1])).map(([node, count]) => (
              <li key={node} className="flex justify-between py-1">
                <span className="text-gray-400">Node {node}</span>
                <span className="font-medium text-gray-200">{count}</span>
              </li>
            ))}
            {Object.keys(perNode).length === 0 && (
              <li className="text-gray-500">No frames in window</li>
            )}
          </ul>
        </Panel>

        <Panel title="Frames per CAN ID">
          <ul className="text-sm text-gray-300">
            {Object.entries(perId).sort((a,b) => Number(b[1]) - Number(a[1])).slice(0, 10).map(([id, count]) => (
              <li key={id} className="flex justify-between py-1">
                <span className="font-mono text-gray-400">0x{Number(id).toString(16).toUpperCase()}</span>
                <span className="font-medium text-gray-200">{count}</span>
              </li>
            ))}
            {Object.keys(perId).length === 0 && (
              <li className="text-gray-500">No frames in window</li>
            )}
          </ul>
        </Panel>
      </div>
    </div>
  );
};

const Stat = ({ label, value }: { label: string; value: string }) => (
  <div>
    <div className="text-xs uppercase text-gray-400 tracking-wide">{label}</div>
    <div className="text-md font-medium text-gray-200">{value}</div>
  </div>
);

const Panel = ({ title, children }: { title: string; children: React.ReactNode }) => (
  <div className="bg-[#252526] rounded-md p-4">
    <div className="text-xs uppercase text-gray-400 tracking-wide mb-2">{title}</div>
    {children}
  </div>
);
