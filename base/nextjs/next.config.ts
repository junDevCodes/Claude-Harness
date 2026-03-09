import type { NextConfig } from 'next';

const nextConfig: NextConfig = {
  images: {
    remotePatterns: [
      // Add your image domains here
      // {
      //   protocol: 'https',
      //   hostname: 'your-s3-bucket.s3.amazonaws.com',
      //   pathname: '/**',
      // },
    ],
    formats: ['image/avif', 'image/webp'],
    minimumCacheTTL: 31536000,
  },
  experimental: {
    cssChunking: 'strict',
  },
  compress: true,
  poweredByHeader: false,
};

export default nextConfig;
