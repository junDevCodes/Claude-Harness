export const API_BASE_URL =
  process.env.EXPO_PUBLIC_API_URL ?? 'http://localhost:8000/api/v1';

export const COLORS = {
  primary: '#0a7ea4',
  primaryLight: '#e0f2fe',
  background: '#f8f9fa',
  surface: '#ffffff',
  text: '#1a1a1a',
  textSecondary: '#6b7280',
  border: '#d1d5db',
  error: '#ef4444',
  success: '#22c55e',
  warning: '#f59e0b',
} as const;

export const SIZES = {
  padding: 24,
  paddingSm: 16,
  radius: 10,
  radiusSm: 6,
  inputHeight: 50,
  buttonHeight: 50,
} as const;

export const TYPOGRAPHY = {
  h1: { fontSize: 28, fontWeight: '700' as const },
  h2: { fontSize: 22, fontWeight: '700' as const },
  h3: { fontSize: 18, fontWeight: '600' as const },
  body: { fontSize: 16 },
  caption: { fontSize: 14 },
  small: { fontSize: 12 },
} as const;
