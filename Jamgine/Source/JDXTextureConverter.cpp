#include <Jamgine\Include\JDXTextureConverter.h>

namespace Jamgine
{

	JDXTextureConverter::JDXTextureConverter(ID3D11Device* p_device)
	{
		m_device = p_device;
		m_lock.clear();
	}

	JDXTextureConverter::~JDXTextureConverter()
	{

	}


	void* JDXTextureConverter::Convert(void* p_data, unsigned int p_size, StackAllocator* p_stack, size_t &p_sizeOut)
	{
		while (m_lock.test_and_set(std::memory_order_acquire))
		{
			//Keep on spinning in the free world
		}
		ID3D11ShaderResourceView* SRV;
		ID3D11Resource* testRes;
		SRV = (ID3D11ShaderResourceView*)p_stack->Push<void>(sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC), 4);
		p_sizeOut = sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC);
		HRESULT re = DirectX::CreateDDSTextureFromMemory(m_device, static_cast<uint8_t*>(p_data), p_size, &testRes /* We don't need this, hopefully....*/, &SRV);

		m_lock.clear();
		return SRV;
	}

}