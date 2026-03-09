import { NextResponse } from "next/server";
import type { NextRequest } from "next/server";

/**
 * Authentication Middleware
 *
 * Protects routes that require authentication.
 * Add paths to PROTECTED_PATHS to require login.
 *
 * Customization:
 *   - PROTECTED_PATHS: paths that require a valid access token
 *   - PUBLIC_PATHS: paths that always bypass the middleware
 *   - ACCESS_TOKEN_COOKIE: cookie name used by the session API route
 */

// Add paths that require authentication (e.g. "/dashboard", "/profile")
const PROTECTED_PATHS: string[] = [
  // "/dashboard",
  // "/profile",
];

// Paths that always bypass middleware checks
const PUBLIC_PATHS = [
  "/",
  "/api",
  "/_next",
  "/favicon.ico",
  "/images",
  "/fonts",
];

const ACCESS_TOKEN_COOKIE = "app_access_token";

export function middleware(request: NextRequest) {
  const { pathname } = request.nextUrl;

  // Skip middleware for public assets and API routes
  if (PUBLIC_PATHS.some((path) => pathname.startsWith(path))) {
    return NextResponse.next();
  }

  const accessToken = request.cookies.get(ACCESS_TOKEN_COOKIE)?.value;
  const isAuthenticated = !!accessToken;

  const isProtectedPath = PROTECTED_PATHS.some((path) =>
    pathname.startsWith(path)
  );

  // Redirect unauthenticated users from protected routes to login
  if (isProtectedPath && !isAuthenticated) {
    const loginUrl = new URL("/login", request.url);
    loginUrl.searchParams.set("redirect", pathname);
    return NextResponse.redirect(loginUrl);
  }

  return NextResponse.next();
}

export const config = {
  matcher: [
    "/((?!_next/static|_next/image|favicon.ico|.*\\.(?:svg|png|jpg|jpeg|gif|webp)$).*)",
  ],
};
