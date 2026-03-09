import { cookies } from "next/headers";
import { NextResponse } from "next/server";

/**
 * Session API Route
 *
 * Manages HTTP-only authentication cookies for secure token storage.
 * Used by AuthProvider to persist login state server-side.
 *
 * POST   /api/auth/session  — Set access + refresh tokens as cookies
 * DELETE /api/auth/session  — Clear cookies (logout)
 * GET    /api/auth/session  — Check if session exists
 */

const ACCESS_TOKEN_COOKIE = "app_access_token";
const REFRESH_TOKEN_COOKIE = "app_refresh_token";

const cookieOptions = {
  httpOnly: true,
  secure: process.env.NODE_ENV === "production",
  sameSite: "lax" as const,
  path: "/",
};

export async function POST(request: Request) {
  try {
    const { accessToken, refreshToken } = await request.json();

    if (!accessToken) {
      return NextResponse.json(
        { error: "Access token is required" },
        { status: 400 }
      );
    }

    const cookieStore = await cookies();

    // Access token — 12 hours
    cookieStore.set(ACCESS_TOKEN_COOKIE, accessToken, {
      ...cookieOptions,
      maxAge: 12 * 60 * 60,
    });

    // Refresh token — 30 days (if provided)
    if (refreshToken) {
      cookieStore.set(REFRESH_TOKEN_COOKIE, refreshToken, {
        ...cookieOptions,
        maxAge: 30 * 24 * 60 * 60,
      });
    }

    return NextResponse.json({ success: true });
  } catch (error) {
    console.error("Session POST error:", error);
    return NextResponse.json(
      { error: "Failed to set session" },
      { status: 500 }
    );
  }
}

export async function DELETE() {
  try {
    const cookieStore = await cookies();
    cookieStore.delete(ACCESS_TOKEN_COOKIE);
    cookieStore.delete(REFRESH_TOKEN_COOKIE);
    return NextResponse.json({ success: true });
  } catch (error) {
    console.error("Session DELETE error:", error);
    return NextResponse.json(
      { error: "Failed to clear session" },
      { status: 500 }
    );
  }
}

export async function GET() {
  try {
    const cookieStore = await cookies();
    const accessToken = cookieStore.get(ACCESS_TOKEN_COOKIE);
    return NextResponse.json({ authenticated: !!accessToken?.value });
  } catch (error) {
    console.error("Session GET error:", error);
    return NextResponse.json({ authenticated: false });
  }
}
