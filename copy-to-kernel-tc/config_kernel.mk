override DEFINE_X86_FEATURE_LA57 := 1
override DISPLAY_CONTROLLER := drm_pdp
override HOST_ALL_ARCH := host_x86_64 host_i386
override LMA := 1
override METAG_VERSION_NEEDED := 2.8.1.0.3
override MIPS_VERSION_NEEDED := 2014.07-1
override PDVFS_COM := PDVFS_COM_HOST
override PDVFS_COM_AP := 2
override PDVFS_COM_HOST := 1
override PDVFS_COM_IMG_CLKDIV := 4
override PDVFS_COM_PMC := 3
override PVRSRV_DIR := services
override PVRSRV_MODNAME := pvrsrvkm
override PVR_ARCH := rogue
override PVR_BUILD_DIR := tc_linux
override PVR_GPIO_MODE := PVR_GPIO_MODE_GENERAL
override PVR_GPIO_MODE_GENERAL := 1
override PVR_GPIO_MODE_POWMON_PIN := 2
override PVR_HANDLE_BACKEND := idr
override PVR_SYSTEM := rgx_linux_tc
override RGX_BNC := 1.V.4.5
override RGX_FW_HEAP_OSID_ASSIGNMENT := RGX_FW_HEAP_USES_FIRMWARE_OSID
override RGX_NUM_DRIVERS_SUPPORTED := 1
override RGX_TIMECORR_CLOCK := mono
override RISCV_VERSION_NEEDED := 1.0.1
override SORT_BRIDGE_STRUCTS := 1
override SUPPORT_BUFFER_SYNC := 1
override SUPPORT_DI_BRG_IMPL := 1
override SUPPORT_DMABUF_BRIDGE := 1
override SUPPORT_DMA_FENCE := 1
override SUPPORT_MIPS_64K_PAGE_SIZE :=
override SUPPORT_NATIVE_FENCE_SYNC := 1
override SUPPORT_PHYSMEM_TEST := 1
override SUPPORT_POWMON_COMPONENT := 1
override SUPPORT_RGX := 1
override SUPPORT_USC_BREAKPOINT := 1
override VMM_TYPE := stub
override WINDOW_SYSTEM := nulldrmws
ifeq ($(CONFIG_DRM_POWERVR_ROGUE_DEBUG),y)
override BUILD := debug
override PVRSRV_ENABLE_GPU_MEMORY_INFO := 1
override PVR_BUILD_TYPE := debug
override SUPPORT_RGXKICKSYNC_BRIDGE := 1
else
override BUILD := release
override PVR_BUILD_TYPE := release
endif
