#include <iostream>
#include <windows.h>

BYTE ShellCodeToMap[12] = {0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xE0};

void SomeFunction()
{
    std::cout << "Function Successfully called from shellcode" << std::endl;
}

class ScMapper
{
public:

    bool MapSc(BYTE ShellCode[], SIZE_T Sz)
    {
        Address = VirtualAlloc(NULL, Sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        if(!memcpy(Address, ShellCode, Sz))
        {
            return false;
            VirtualFree(Address, Sz, MEM_DECOMMIT | MEM_RELEASE);
        }

        return true;
    }

    void ExecuteSc()
    {
        ((void(*)())Address)();
    }

    bool UnmapSc(SIZE_T Sz)
    {
        if(!VirtualFree(Address, Sz, MEM_RELEASE | MEM_DECOMMIT))
        {
            return false;
        }

        return true;
    }

private:
    void* Address = nullptr;
};

int main()
{
    void* address = (void*)SomeFunction;
    memcpy(&ShellCodeToMap[2], &address, sizeof(void*));

    std::cout << "Size: " << sizeof(void*) << std::endl;
    std::cout << "Function Address: " << (void*)SomeFunction << std::endl;

    for(int i = 0; i < 12; i++)
    {
        printf("%02x ", ShellCodeToMap[i]);
    }

    std::cout << std::endl;

    ScMapper SCM;
    SCM.MapSc(ShellCodeToMap, sizeof(ShellCodeToMap));
    //SCM.ExecuteSc();
    system("pause");
    return 0;
}