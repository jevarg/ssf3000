#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler
#include <cassert>

ID3D11Device *device;

ID3D11VertexShader *vertex_shader_ptr = nullptr;
ID3D11PixelShader *pixel_shader_ptr = nullptr;

void dxCompileShader() {
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif
    ID3DBlob *vs_blob_ptr = NULL, *ps_blob_ptr = NULL, *error_blob = NULL;

    // COMPILE VERTEX SHADER
    HRESULT hr = D3DCompileFromFile(
            L"shaders/default.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "vs_main",
            "vs_5_0",
            flags,
            0,
            &vs_blob_ptr,
            &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            OutputDebugStringA((char *) error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if (vs_blob_ptr) { vs_blob_ptr->Release(); }
        assert(false);
    }

    // COMPILE PIXEL SHADER
    hr = D3DCompileFromFile(
            L"shaders.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "ps_main",
            "ps_5_0",
            flags,
            0,
            &ps_blob_ptr,
            &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            OutputDebugStringA((char *) error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if (ps_blob_ptr) { ps_blob_ptr->Release(); }
        assert(false);
    }

    hr = device->CreateVertexShader(
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            nullptr,
            &vertex_shader_ptr);
    assert(SUCCEEDED(hr));

    hr = device->CreatePixelShader(
            ps_blob_ptr->GetBufferPointer(),
            ps_blob_ptr->GetBufferSize(),
            nullptr,
            &pixel_shader_ptr);
    assert(SUCCEEDED(hr));

    ID3D11InputLayout *input_layout_ptr = nullptr;
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            /*
            { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            */
    };
    hr = device->CreateInputLayout(
            inputElementDesc,
            ARRAYSIZE(inputElementDesc),
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            &input_layout_ptr);
    assert(SUCCEEDED(hr));
}

void dxInit(HWND hwnd) {
    ID3D11DeviceContext *deviceCtx;
    IDXGISwapChain *swapChain;
    ID3D11RenderTargetView *renderTarget;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = true;

    D3D_FEATURE_LEVEL featureLvl;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#if defined(DEBUG) || defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &swapChain,
            &device,
            &featureLvl,
            &deviceCtx);
    assert(hr == S_OK && swapChain && device && deviceCtx);

    ID3D11Texture2D *framebuffer;
    hr = swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            (void **) &framebuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateRenderTargetView(
            framebuffer,
            nullptr,
            &renderTarget);
    assert(SUCCEEDED(hr));

    framebuffer->Release();


    // Shaders
    dxCompileShader();

}