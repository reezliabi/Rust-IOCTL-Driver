#include "iletisimyontemi.h"
#include "skStr.h"

#define drv_device skCrypt(L"\\Device\\csrss.exe")
#define drv_dos_device skCrypt(L"\\DosDevices\\csrss.exe")
#define drv skCrypt(L"\\Driver\\csrss.exe")

NTSTATUS io_device_control(PDEVICE_OBJECT device, PIRP irp) {

    ULONG ioctlcode = IoGetCurrentIrpStackLocation(irp)->Parameters.DeviceIoControl.IoControlCode;
    NTSTATUS status = STATUS_INVALID_PARAMETER;
    ULONG infosize = 0;

    if (ioctlcode == phy_memory) {
        pk_rw_memoryrequest in = pk_rw_memoryrequest(irp->AssociatedIrp.SystemBuffer);
        PVOID kernel_buff = ExAllocatePool(NonPagedPool, in->size);
        if (kernel_buff)
        {
            if (in->memo == true)
                memcpy(kernel_buff, in->output, in->size);

            status = memory_kernel_memory((HANDLE)in->pid, in->address, kernel_buff, in->size, in->memo);

            if (NT_SUCCESS(status) && in->memo == false)
                memcpy(in->output, kernel_buff, in->size);

            ExFreePoolWithTag(kernel_buff, 0);
        }
        infosize = sizeof(k_rw_memoryrequest);
    }
    irp->IoStatus.Status = status;
    irp->IoStatus.Information = infosize;
    IofCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS ioctl_create(PDEVICE_OBJECT device, PIRP irp) {
    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;
    IofCompleteRequest(irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS init(PDRIVER_OBJECT driver, PUNICODE_STRING path) {
    UNICODE_STRING dev;
    UNICODE_STRING dos;
    RtlInitUnicodeString(&dev, drv_device);
    RtlInitUnicodeString(&dos, drv_dos_device);

    IoCreateDevice(driver, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &driver->DeviceObject);

    IoCreateSymbolicLink(&dos, &dev);

    driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = io_device_control;
    driver->MajorFunction[IRP_MJ_CREATE] = ioctl_create;

    driver->DeviceObject->Flags |= DO_DIRECT_IO;
    driver->DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    phy_memory = static_cast<uintptr_t>(CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8122468, METHOD_BUFFERED, FILE_SPECIAL_ACCESS));

    ObfDereferenceObject(driver);

    return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING path) {
    UNREFERENCED_PARAMETER(driver);
    UNREFERENCED_PARAMETER(path);
    UNICODE_STRING drv_name;
    RtlInitUnicodeString(&drv_name, drv);
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    status = IoCreateDriver(&drv_name, &init);
    return status;
}
