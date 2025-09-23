import type { CANFrame } from '../hooks/useWebSocket';

type Props = {
  frames: CANFrame[];
};

export const MessageTable = ({ frames }: Props) => {
  return (
    <div className="bg-[#252526] rounded-md overflow-hidden">
      <table className="w-full text-sm">
        <thead className="bg-[#2d2d30]">
          <tr className="text-left text-gray-400">
            <th className="px-4 py-2 font-normal">Time</th>
            <th className="px-4 py-2 font-normal">Node</th>
            <th className="px-4 py-2 font-normal">ID</th>
            <th className="px-4 py-2 font-normal">DLC</th>
            <th className="px-4 py-2 font-normal">Data</th>
          </tr>
        </thead>

        <tbody>
          {frames.map((f, i) => (
            <tr
              key={i}
              className="border-t border-[#2a2a2a] hover:bg-[#2a2d2e]"
            >
              <td className="px-4 py-2 text-gray-500">
                {new Date(f.ts).toLocaleTimeString()}
              </td>

              <td className="px-4 py-2 text-[#FFFFB8]">
                Node {f.node}
              </td>

              <td className="px-4 py-2 font-mono text-gray-200">
                0x{f.id.toString(16).toUpperCase()}
              </td>

              <td className="px-4 py-2 text-gray-300">
                {f.dlc}
              </td>

              <td className="px-4 py-2 font-mono text-gray-400">
                {f.data.map(b => b.toString(16).padStart(2, '0')).join(' ')}
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};
