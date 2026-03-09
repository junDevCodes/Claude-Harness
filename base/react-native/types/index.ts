// ─────────────────────────────────────────
// Domain types
// ─────────────────────────────────────────

export interface User {
  id: number;
  email: string;
  username: string;
  created_at: string;
}

// ─────────────────────────────────────────
// Auth types
// ─────────────────────────────────────────

export interface AuthTokens {
  access_token: string;
  refresh_token: string;
}

export interface LoginRequest {
  email: string;
  password: string;
}

export interface RegisterRequest {
  email: string;
  username: string;
  password: string;
}

export interface RefreshTokenRequest {
  refresh_token: string;
}

// ─────────────────────────────────────────
// API response types
// ─────────────────────────────────────────

export interface ApiError {
  detail: string;
  status_code?: number;
}

export interface PaginatedResponse<T> {
  items: T[];
  total: number;
  page: number;
  size: number;
  pages: number;
}
