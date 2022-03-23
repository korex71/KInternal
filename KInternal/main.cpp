#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <thread>

namespace offsets
{
	// local client;
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDB35DC;
	constexpr ::std::ptrdiff_t dwForceJump = 0x5278DDC;

	
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
}

void Bhop(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	// loop cheat;
	while (!GetAsyncKeyState(VK_END))
	{
		::std::this_thread::sleep_for(::std::chrono::milliseconds(1));

		if (!GetAsyncKeyState(VK_SPACE))
			continue;

		const auto LocalPlayer = *reinterpret_cast<::std::uintptr_t*>(client + offsets::dwLocalPlayer);

		// player exists;
		if (!LocalPlayer)
			continue;

		const auto Health = *reinterpret_cast<::std::int32_t*>(LocalPlayer + offsets::m_iHealth);

		// player is alive;
		if (!Health)
			return;

		const auto flags = *reinterpret_cast<::std::int32_t*>(LocalPlayer + offsets::m_fFlags);


		// player on ground; ? force : back
		(flags & (1 << 0)) ? *reinterpret_cast<::std::uintptr_t*>(client + offsets::dwForceJump) = 6 : *reinterpret_cast<::std::uintptr_t*>(client + offsets::dwForceJump) = 4;
	}


	// uninject cheat;
	FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	
	if (reason == 1) // dll link;
	{
		DisableThreadLibraryCalls(instance);

		// cheat thread;
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Bhop),
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return 1;
}