//
// Created by TheAwesomeGem on 11/29/2025.
//

#include "RenderDebug.h"

#include <comdef.h>
#include <dxgi1_3.h>

#include "RendererCommon.h"
#include "src/Log.h"

using RendererCommon::LOG_CAT;

void RenderDebug::init()
{
    check(DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_debug_info.put())));

    if (!m_debug_info)
    {
        Log::fatal(LOG_CAT, "Failed getting debug interface.");

        // ReSharper disable once CppRedundantControlFlowJump
        return;
    }
}

void RenderDebug::process_debug_messages() const
{
    const GUID log_level = DXGI_DEBUG_ALL;

    if (!m_debug_info)
    {
        return;
    }

    const uint64_t message_count = m_debug_info->GetNumStoredMessages(log_level);

    for (uint64_t i = 0; i < message_count; ++i)
    {
        size_t message_len = 0;
        HRESULT log_result = m_debug_info->GetMessageA(log_level, i, nullptr, &message_len);

        if (FAILED(log_result))
        {
            Log::fatal(LOG_CAT, "Failed to get log messages.");

            continue;
        }

        std::vector<byte> message_buffer;
        message_buffer.reserve(message_len);
        DXGI_INFO_QUEUE_MESSAGE* message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(message_buffer.data());
        log_result = m_debug_info->GetMessageA(log_level, i, message, &message_len);

        if (FAILED(log_result))
        {
            Log::fatal(LOG_CAT, "Failed to get log messages.");

            continue;
        }

        auto get_category_name = [](DXGI_INFO_QUEUE_MESSAGE_CATEGORY category)
        {
            switch (category)
            {
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN:
                    return "UNKN";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS:
                    return "MISC";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION:
                    return "INIT";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP:
                    return "CLEAN";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION:
                    return "COMPL";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION:
                    return "STCRT";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING:
                    return "STSET";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING:
                    return "STGET";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
                    return "RESMA";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION:
                    return "EXECU";
                case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER:
                    return "SHADR";
                default:
                    return "UNKN";
            }
        };

        switch (message->Severity)
        {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
            {
                Log::fatal(LOG_CAT, "{} {}", get_category_name(message->Category), message->pDescription);
                break;
            }
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
            {
                Log::error(LOG_CAT, "{} {}", get_category_name(message->Category), message->pDescription);
                break;
            }
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
            {
                Log::warn(LOG_CAT, "{} {}", get_category_name(message->Category), message->pDescription);
                break;
            }
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
            {
                Log::info(LOG_CAT, "{} {}", get_category_name(message->Category), message->pDescription);
                break;
            }
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
            {
                Log::debug(LOG_CAT, "{} {}", get_category_name(message->Category), message->pDescription);
                break;
            }
        }
    }

    m_debug_info->ClearStoredMessages(log_level);
}

void RenderDebug::check(HRESULT hr) const
{
    if (FAILED(hr))
    {
        process_debug_messages();

        const _com_error err = _com_error{ hr };
        Log::fatal(LOG_CAT, "Error. hr=0x{:x}. Reason: {}", err.WCode(), err.ErrorMessage());
    }
}

void RenderDebug::sh_check(HRESULT res, ID3DBlob* error_message) const
{
    if (FAILED(res) || error_message != nullptr)
    {
        process_debug_messages();

        if (error_message != nullptr)
        {
            const char* msg = static_cast<const char*>(error_message->GetBufferPointer());
            const size_t len = error_message->GetBufferSize();

            Log::fatal(LOG_CAT, "Shader compile error:\n{}\n", std::string_view{ msg, len });
        }
    }
}
