/**
 * API Client with JWT Authentication
 *
 * Features:
 * - Automatic Bearer token injection
 * - 401 auto-retry with token refresh
 * - HTTP-only cookie sync for SSR/middleware
 * - Typed error handling
 *
 * Customization:
 * - Update API_BASE_URL to match your backend
 * - Update LoginResponse field names to match your API
 * - Add domain-specific API methods below
 */

const isDev = process.env.NEXT_PUBLIC_ENV === "development";
const API_BASE_URL = isDev
  ? (process.env.NEXT_PUBLIC_API_URL_DEV || "http://localhost:8000")
  : (process.env.NEXT_PUBLIC_API_URL_PROD || "http://localhost:8000");

// ============================================
// Types
// ============================================

export interface LoginResponse {
  user_id: string;
  access_token: string;
  refresh_token: string;
  username: string | null;
}

export interface UserInfo {
  id: string;
  username: string;
  email: string | null;
  auth_type: string;
  is_admin: boolean;
}

export interface RefreshTokenResponse {
  access_token: string;
  refresh_token: string;
}

// ============================================
// Token Management
// ============================================

const ACCESS_TOKEN_KEY = "app_access_token";
const REFRESH_TOKEN_KEY = "app_refresh_token";

export function getAccessToken(): string | null {
  if (typeof window === "undefined") return null;
  return localStorage.getItem(ACCESS_TOKEN_KEY);
}

export function getRefreshToken(): string | null {
  if (typeof window === "undefined") return null;
  return localStorage.getItem(REFRESH_TOKEN_KEY);
}

/**
 * Store tokens in localStorage and sync to HTTP-only cookies for SSR.
 */
export function setTokens(accessToken: string, refreshToken: string): void {
  if (typeof window === "undefined") return;
  localStorage.setItem(ACCESS_TOKEN_KEY, accessToken);
  localStorage.setItem(REFRESH_TOKEN_KEY, refreshToken);
  syncTokensToCookies(accessToken, refreshToken);
}

async function syncTokensToCookies(
  accessToken: string,
  refreshToken: string
): Promise<void> {
  try {
    await fetch("/api/auth/session", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ accessToken, refreshToken }),
    });
  } catch (error) {
    console.error("Failed to sync tokens to cookies:", error);
  }
}

export function clearTokens(): void {
  if (typeof window === "undefined") return;
  localStorage.removeItem(ACCESS_TOKEN_KEY);
  localStorage.removeItem(REFRESH_TOKEN_KEY);
  clearTokenCookies();
}

async function clearTokenCookies(): Promise<void> {
  try {
    await fetch("/api/auth/session", { method: "DELETE" });
  } catch (error) {
    console.error("Failed to clear token cookies:", error);
  }
}

export function hasTokens(): boolean {
  return getAccessToken() !== null;
}

// ============================================
// Error Handling
// ============================================

export class ApiError extends Error {
  constructor(
    public status: number,
    public statusText: string,
    public data?: unknown
  ) {
    super(`API Error: ${status} ${statusText}`);
    this.name = "ApiError";
  }
}

// ============================================
// Core Request Function
// ============================================

async function refreshAccessToken(): Promise<boolean> {
  const refreshToken = getRefreshToken();
  if (!refreshToken) return false;

  try {
    const response = await fetch(`${API_BASE_URL}/api/v1/auth/refresh`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ refresh_token: refreshToken }),
    });

    if (!response.ok) {
      clearTokens();
      return false;
    }

    const data: RefreshTokenResponse = await response.json();
    setTokens(data.access_token, data.refresh_token);
    return true;
  } catch {
    clearTokens();
    return false;
  }
}

async function apiRequest<T>(
  endpoint: string,
  options: RequestInit = {},
  retry = true
): Promise<T> {
  const accessToken = getAccessToken();

  const headers: HeadersInit = {
    "Content-Type": "application/json",
    ...options.headers,
  };

  if (accessToken) {
    (headers as Record<string, string>)["Authorization"] = `Bearer ${accessToken}`;
  }

  const response = await fetch(`${API_BASE_URL}${endpoint}`, {
    ...options,
    headers,
  });

  // Auto-refresh on 401
  if (response.status === 401 && retry) {
    const refreshed = await refreshAccessToken();
    if (refreshed) {
      return apiRequest<T>(endpoint, options, false);
    }
    throw new ApiError(401, "Unauthorized", { needsLogin: true });
  }

  if (!response.ok) {
    const data = await response.json().catch(() => null);
    throw new ApiError(response.status, response.statusText, data);
  }

  const text = await response.text();
  if (!text) return {} as T;
  return JSON.parse(text) as T;
}

// ============================================
// Auth API
// ============================================

export async function emailLogin(
  email: string,
  password: string
): Promise<LoginResponse> {
  return apiRequest<LoginResponse>("/api/v1/auth/login", {
    method: "POST",
    body: JSON.stringify({ email, password }),
  });
}

export async function emailSignUp(
  email: string,
  password: string,
  username: string
): Promise<LoginResponse> {
  return apiRequest<LoginResponse>("/api/v1/auth/sign-up", {
    method: "POST",
    body: JSON.stringify({ email, password, username }),
  });
}

export async function getCurrentUser(): Promise<UserInfo> {
  return apiRequest<UserInfo>("/api/v1/auth/me");
}

export async function refreshTokens(): Promise<RefreshTokenResponse> {
  const refreshToken = getRefreshToken();
  if (!refreshToken) throw new ApiError(401, "No refresh token");
  return apiRequest<RefreshTokenResponse>("/api/v1/auth/refresh", {
    method: "POST",
    body: JSON.stringify({ refresh_token: refreshToken }),
  });
}

// ============================================
// Generic API Methods
// Extend with your domain-specific API calls
// ============================================

export const api = {
  get: <T>(endpoint: string) => apiRequest<T>(endpoint),
  post: <T>(endpoint: string, body: unknown) =>
    apiRequest<T>(endpoint, { method: "POST", body: JSON.stringify(body) }),
  put: <T>(endpoint: string, body: unknown) =>
    apiRequest<T>(endpoint, { method: "PUT", body: JSON.stringify(body) }),
  patch: <T>(endpoint: string, body: unknown) =>
    apiRequest<T>(endpoint, { method: "PATCH", body: JSON.stringify(body) }),
  delete: <T>(endpoint: string) =>
    apiRequest<T>(endpoint, { method: "DELETE" }),
};
