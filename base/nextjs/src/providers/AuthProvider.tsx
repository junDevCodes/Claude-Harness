"use client";

import {
  createContext,
  useCallback,
  useContext,
  useEffect,
  useState,
  type ReactNode,
} from "react";
import {
  clearTokens,
  getCurrentUser,
  hasTokens,
  setTokens,
  emailLogin as apiEmailLogin,
  emailSignUp as apiEmailSignUp,
  type LoginResponse,
  type UserInfo,
} from "@/lib/api";

// ============================================
// Types
// ============================================

interface AuthContextType {
  user: UserInfo | null;
  isLoading: boolean;
  isAuthenticated: boolean;
  emailLogin: (email: string, password: string) => Promise<void>;
  emailSignUp: (email: string, password: string, username: string) => Promise<void>;
  login: (loginResponse: LoginResponse) => void;
  logout: () => Promise<void>;
  refreshUser: () => Promise<void>;
}

const AuthContext = createContext<AuthContextType | undefined>(undefined);

// ============================================
// Provider Component
// ============================================

export function AuthProvider({ children }: { children: ReactNode }) {
  const [user, setUser] = useState<UserInfo | null>(null);
  const [isLoading, setIsLoading] = useState(true);

  const isAuthenticated = user !== null;

  /**
   * Fetch current user info from backend API.
   */
  const refreshUser = useCallback(async () => {
    if (!hasTokens()) {
      setUser(null);
      setIsLoading(false);
      return;
    }

    try {
      const userInfo = await getCurrentUser();
      setUser(userInfo);
    } catch (error) {
      console.error("Failed to fetch user:", error);
      if (hasTokens()) clearTokens();
      setUser(null);
    } finally {
      setIsLoading(false);
    }
  }, []);

  /**
   * Store tokens and update user state from any login method.
   */
  const login = useCallback(
    (loginResponse: LoginResponse) => {
      setTokens(loginResponse.access_token, loginResponse.refresh_token);
      setUser({
        id: loginResponse.user_id,
        username: loginResponse.username || "",
        email: null,
        auth_type: "email",
        is_admin: false,
      });
      setIsLoading(false);
      refreshUser();
    },
    [refreshUser]
  );

  /**
   * Email/password login.
   */
  const emailLogin = useCallback(
    async (email: string, password: string) => {
      setIsLoading(true);
      try {
        const response = await apiEmailLogin(email, password);
        login(response);
      } catch (error) {
        setIsLoading(false);
        throw error;
      }
    },
    [login]
  );

  /**
   * Email/password sign-up.
   */
  const emailSignUp = useCallback(
    async (email: string, password: string, username: string) => {
      setIsLoading(true);
      try {
        const response = await apiEmailSignUp(email, password, username);
        login(response);
      } catch (error) {
        setIsLoading(false);
        throw error;
      }
    },
    [login]
  );

  /**
   * Logout — clear tokens and user state.
   */
  const logout = useCallback(async () => {
    setUser(null);
    clearTokens();
  }, []);

  // Initialize auth state on mount
  useEffect(() => {
    refreshUser();
  }, [refreshUser]);

  // Sync auth state across browser tabs
  useEffect(() => {
    const handleStorageChange = (e: StorageEvent) => {
      if (e.key === "app_access_token") {
        if (!e.newValue) {
          setUser(null);
        } else {
          refreshUser();
        }
      }
    };

    window.addEventListener("storage", handleStorageChange);
    return () => window.removeEventListener("storage", handleStorageChange);
  }, [refreshUser]);

  return (
    <AuthContext.Provider
      value={{
        user,
        isLoading,
        isAuthenticated,
        emailLogin,
        emailSignUp,
        login,
        logout,
        refreshUser,
      }}
    >
      {children}
    </AuthContext.Provider>
  );
}

// ============================================
// Hooks
// ============================================

/**
 * Access the auth context. Must be used inside AuthProvider.
 */
export function useAuth() {
  const context = useContext(AuthContext);
  if (context === undefined) {
    throw new Error("useAuth must be used within an AuthProvider");
  }
  return context;
}

/**
 * Convenience hook for authentication status.
 */
export function useIsAuthenticated() {
  const { isAuthenticated, isLoading } = useAuth();
  return { isAuthenticated, isLoading };
}
