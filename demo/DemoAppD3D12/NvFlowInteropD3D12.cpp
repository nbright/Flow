/*
 * Copyright (c) 2014-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

#include <d3d12.h>

#include "../DemoApp/NvFlowInterop.h"

#include "NvFlowContextD3D12.h"
#include "appD3D12Ctx.h"

NvFlowDescriptorReserveHandleD3D12 NvFlowInteropReserveDescriptors(void* userdata, NvFlowUint numDescriptors, NvFlowUint64 lastFenceCompleted, NvFlowUint64 nextFenceValue)
{
	auto appctx = static_cast<AppGraphCtx*>(userdata);
	auto srcHandle = appctx->m_dynamicHeapCbvSrvUav.reserveDescriptors(numDescriptors, lastFenceCompleted, nextFenceValue);
	NvFlowDescriptorReserveHandleD3D12 handle = {};
	handle.heap = srcHandle.heap;
	handle.descriptorSize = srcHandle.descriptorSize;
	handle.cpuHandle = srcHandle.cpuHandle;
	handle.gpuHandle = srcHandle.gpuHandle;
	return handle;
}

void NvFlowInteropUpdateContextDesc(NvFlowContextDescD3D12* desc, AppGraphCtx* appctx)
{
	desc->device = appctx->m_device;
	desc->commandQueue = appctx->m_commandQueue;
	desc->commandQueueFence = appctx->m_fence;
	desc->commandList = appctx->m_commandList;
	desc->lastFenceCompleted = appctx->m_lastFenceComplete;
	desc->nextFenceValue = appctx->m_thisFrameFenceID;

	desc->dynamicHeapCbvSrvUav.userdata = appctx;
	desc->dynamicHeapCbvSrvUav.reserveDescriptors = NvFlowInteropReserveDescriptors;
}

NvFlowContext* NvFlowInteropCreateContext(AppGraphCtx* appctx)
{
	NvFlowContextDescD3D12 desc = {};
	NvFlowInteropUpdateContextDesc(&desc, appctx);
	return NvFlowCreateContextD3D12(NV_FLOW_VERSION, &desc);
}

NvFlowDepthStencilView* NvFlowInteropCreateDepthStencilView(AppGraphCtx* appctx, NvFlowContext* flowctx)
{
	NvFlowDepthStencilViewDescD3D12 desc = {};
	desc.dsvHandle = appctx->m_current_dsvHandle;
	desc.dsvDesc = appctx->m_current_dsvDesc;
	desc.srvHandle = appctx->m_current_depth_srvHandle;
	desc.srvDesc = appctx->m_current_depth_srvDesc;
	desc.resource = appctx->m_depthStencil;
	desc.currentState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	desc.viewport = appctx->m_viewport;
	return NvFlowCreateDepthStencilViewD3D12(flowctx, &desc);
}

NvFlowRenderTargetView* NvFlowInteropCreateRenderTargetView(AppGraphCtx* appctx, NvFlowContext* flowctx)
{
	NvFlowRenderTargetViewDescD3D12 desc = {};
	desc.rtvHandle = appctx->m_current_rtvHandle;
	desc.rtvDesc = appctx->m_current_rtvDesc;
	desc.resource = appctx->m_current_renderTarget;
	desc.currentState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	desc.viewport = appctx->m_viewport;
	desc.scissor = appctx->m_scissorRect;
	return NvFlowCreateRenderTargetViewD3D12(flowctx, &desc);
}

void NvFlowInteropUpdateContext(NvFlowContext* context, AppGraphCtx* appctx)
{
	NvFlowContextDescD3D12 desc = {};
	NvFlowInteropUpdateContextDesc(&desc, appctx);
	NvFlowUpdateContextD3D12(context, &desc);
}

void NvFlowInteropUpdateDepthStencilView(NvFlowDepthStencilView* view, AppGraphCtx* appctx, NvFlowContext* flowctx)
{
	NvFlowDepthStencilViewDescD3D12 desc = {};
	desc.dsvHandle = appctx->m_current_dsvHandle;
	desc.dsvDesc = appctx->m_current_dsvDesc;
	desc.srvHandle = appctx->m_current_depth_srvHandle;
	desc.srvDesc = appctx->m_current_depth_srvDesc;
	desc.resource = appctx->m_depthStencil;
	desc.currentState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	desc.viewport = appctx->m_viewport;
	NvFlowUpdateDepthStencilViewD3D12(flowctx, view, &desc);
}

void NvFlowInteropUpdateRenderTargetView(NvFlowRenderTargetView* view, AppGraphCtx* appctx, NvFlowContext* flowctx)
{
	NvFlowRenderTargetViewDescD3D12 desc = {};
	desc.rtvHandle = appctx->m_current_rtvHandle;
	desc.rtvDesc = appctx->m_current_rtvDesc;
	desc.resource = appctx->m_current_renderTarget;
	desc.currentState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	desc.viewport = appctx->m_viewport;
	desc.scissor = appctx->m_scissorRect;
	NvFlowUpdateRenderTargetViewD3D12(flowctx, view, &desc);
}