#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(location = 0) uniform float dt;

layout(binding = 0) uniform sampler2D in_field;
layout(rgba16f, binding = 1) uniform image2D out_field;
layout(binding = 2) uniform sampler2D velocity_field;



// vec4 bilinear_interpolate_sampler(sampler2D t, vec2 uv)
// {
//   vec2 res = textureSize( t, 0);

//   vec2 st = uv;

//   vec2 iuv = floor( st );
//   vec2 fuv = fract( st );

//   vec4 a = texture2D( t, (iuv+vec2(0.5,0.5))/res );
//   vec4 b = texture2D( t, (iuv+vec2(1.5,0.5))/res );
//   vec4 c = texture2D( t, (iuv+vec2(0.5,1.5))/res );
//   vec4 d = texture2D( t, (iuv+vec2(1.5,1.5))/res );

//   return mix( mix( a, b, fuv.x),
//                       mix( c, d, fuv.x), fuv.y );
// }



vec4 bilinear_interpolate_sampler(sampler2D field, vec2 st)
{
    ivec2 tex_size = textureSize(in_field, 0);
    st = clamp(st, ivec2(0,0), tex_size.xy - 1);
    
    ivec2 iuv = ivec2(floor( st ));
    vec2 fuv = fract( st );
    const ivec2 dx = ivec2(1, 0);
    const ivec2 dy = ivec2(0, 1);


    vec4 a,b,c,d;
    a = texelFetch(field, iuv, 0);
    b = texelFetchOffset(field, iuv, 0, dx);
    c = texelFetchOffset(field, iuv, 0, dy);
    d = texelFetchOffset(field, iuv, 0, dx + dy);

    return mix( mix( a, b, fuv.x),
                mix( c, d, fuv.x), fuv.y );
}



// http://www.sci.utah.edu/~cscheid/scivis_fall07/2d_vector_vis.pdf
// allow u(x,t) = u(x, t + dt * 0.5) = u(x, t + dt)
vec2 simplified_fourth_order_runge_kutta_trace(ivec2 pos){
    
    vec2 v1 = bilinear_interpolate_sampler(velocity_field, pos).xy;
    vec2 v2 = bilinear_interpolate_sampler(velocity_field, pos - v1 * dt * 0.5).xy;
    vec2 v3 = bilinear_interpolate_sampler(velocity_field, pos - v2 * dt * 0.5).xy;
    vec2 v4 = bilinear_interpolate_sampler(velocity_field, pos - v3 *  dt).xy;

    return (v1 + 2 * (v2 + v3) + v4) * 1 / 6.0;
}

vec4 euler_trace(ivec2 pos){
    return texelFetch(velocity_field, pos, 0);
}


void main(){
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec2 velocity = simplified_fourth_order_runge_kutta_trace(pos).xy;
    vec4 quantity = bilinear_interpolate_sampler(in_field, pos - dt * velocity);
    imageStore(out_field, pos, quantity);
    

}