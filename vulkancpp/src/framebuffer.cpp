#include "framebuffer.hpp"

void createFrameBuffers(const FrameBufferIn& in, std::vector<SwapChainFrame>& frames, const bool& debug)
{
  for(int i = 0; i < frames.size(); i++)
  {
    std::array<vk::ImageView, 1> attachments = {frames[i].imageView};
    vk::FramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.renderPass = in.renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = in.extent.width;
    framebufferInfo.height = in.extent.height;
    framebufferInfo.layers = 1;

    try
    {
      frames[i].framebuffer = in.device.createFramebuffer(framebufferInfo);
      if(debug)
      {
        std::cout << "Framebuffer created " << i << "\n";
      }
    }
    catch(vk::SystemError& e)
    {
      std::cerr << e.what() << '\n';
      throw std::runtime_error("Failed to create framebuffer " + std::to_string(i) +"\n");
    }
  }
}