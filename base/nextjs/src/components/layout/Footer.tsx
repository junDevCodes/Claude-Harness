export function Footer() {
  return (
    <footer className="bg-gray-900 text-white/80 py-12 px-6">
      <div className="max-w-5xl mx-auto">
        <div className="mb-8">
          <span className="text-xl font-bold text-white">My App</span>
        </div>
        <div className="pt-6 border-t border-white/10">
          <p className="text-[12px] text-white/30">
            &copy; {new Date().getFullYear()} My App. All rights reserved.
          </p>
        </div>
      </div>
    </footer>
  );
}
