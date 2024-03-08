#pragma once

namespace SW {

	enum StatusCode
	{
		// Success
		Success = 0,
		Success_HostAlreadyInitialized = 0x00000001,
		Success_DifferentRuntimeProperties = 0x00000002,

		// Failure
		Failure_InvalidArg = 0x80008081,
		Failure_CoreHostLibLoad = 0x80008082,
		Failure_CoreHostLibMissing = 0x80008083,
		Failure_CoreHostEntryPoint = 0x80008084,
		Failure_CoreHostCurHostFind = 0x80008085,

		Failure_CoreClrResolve = 0x80008087,
		Failure_CoreClrBind = 0x80008088,
		Failure_CoreClrInit = 0x80008089,
		Failure_CoreClrExe = 0x8000808a,
		Failure_ResolverInit = 0x8000808b,
		Failure_ResolverResolve = 0x8000808c,
		Failure_LibHostCurExeFind = 0x8000808d,
		Failure_LibHostInit = 0x8000808e,

		Failure_LibHostExecMode = 0x80008090,
		Failure_LibHostSdkFind = 0x80008091,
		Failure_LibHostInvalidArgs = 0x80008092,
		Failure_InvalidConfigFile = 0x80008093,
		Failure_AppArgNotRunnable = 0x80008094,
		Failure_AppHostExeNotBound = 0x80008095,
		Failure_FrameworkMissing = 0x80008096,
		Failure_HostApiFailed = 0x80008097,
		Failure_HostApiBufferTooSmall = 0x80008098,
		Failure_LibHostUnknownCommand = 0x80008099,
		Failure_LibHostAppRootFind = 0x8000809a,
		Failure_SdkResolverResolve = 0x8000809b,
		Failure_FrameworkCompat = 0x8000809c,
		Failure_FrameworkCompatRetry = 0x8000809d,

		Failure_BundleExtraction = 0x8000809f,
		Failure_BundleExtractionIOError = 0x800080a0,
		Failure_LibHostDuplicateProperty = 0x800080a1,
		Failure_HostApiUnsupportedVersion = 0x800080a2,
		Failure_HostInvalidState = 0x800080a3,
		Failure_HostPropertyNotFound = 0x800080a4,
		Failure_CoreHostIncompatibleConfig = 0x800080a5,
		Failure_HostApiUnsupportedScenario = 0x800080a6,
		Failure_HostFeatureDisabled = 0x800080a7,
	};

}
