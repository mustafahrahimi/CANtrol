import { useEffect, useState, useRef } from 'react';

// CAN frame type
export type CANFrame = {
  id: number;
  dlc: number;
  data: number[];
  node: number;
  ts: number; // arrival timestamp (ms since epoch)
};

export function useWebSocket(url: string) {
  const [frames, setFrames] = useState<CANFrame[]>([]);
  const [totalCount, setTotalCount] = useState<number>(0);
  const [lastTs, setLastTs] = useState<number | null>(null);
  const wsRef = useRef<WebSocket | null>(null);

  useEffect(() => {
    const ws = new WebSocket(url);
    wsRef.current = ws;

    ws.onopen = () => console.log('WebSocket connected');
    ws.onclose = () => console.log('WebSocket disconnected');

    ws.onmessage = (event) => {
      try {
        const parsed = JSON.parse(event.data) as Omit<CANFrame, 'ts'>;
        const frame: CANFrame = { ...parsed, ts: Date.now() };
        // Keep only last 200 frames to improve FPS/bus load windows
        setFrames((prev) => [frame, ...prev].slice(0, 200));
        setTotalCount((c) => c + 1);
        setLastTs(frame.ts);
      } catch (err) {
        console.error('Failed to parse frame', err);
      }
    };

    return () => ws.close();
  }, [url]);

  return { frames, totalCount, lastTs };
}

