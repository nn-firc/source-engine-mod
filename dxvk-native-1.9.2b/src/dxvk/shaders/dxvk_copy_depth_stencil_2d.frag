#version 450

#extension GL_ARB_shader_stencil_export : enable

layout(set = 0, binding = 0)
uniform sampler2DArray s_depth;

layout(set = 0, binding = 1)
uniform usampler2DArray s_stencil;

layout(push_constant)
uniform u_info_t {
  ivec2 offset;
} u_info;

void main() {
  ivec3 coord = ivec3(gl_FragCoord.xy + u_info.offset.xy, gl_Layer);
  gl_FragDepth         = texelFetch(s_depth,   coord, 0).r;
  gl_FragStencilRefARB = int(texelFetch(s_stencil, coord, 0).r);
}