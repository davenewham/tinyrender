R""(

struct VertexInput {
    @location(0) position: vec3f,
    @location(1) data: f32,
};
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) voxelValue: f32,
    @location(1) ambientOcclusion: f32,
};

struct MyUniforms {
    projectionMatrix: mat4x4f,
    viewMatrix: mat4x4f,
    cameraWorldPosition: vec3f,
};

struct LightingUniforms {
    directions: array<vec4<f32>, 2>,
    colors: array<vec4<f32>, 2>,
    ambient: f32,
};

@group(0) @binding(0) var<uniform> uMyUniforms: MyUniforms;
@group(0) @binding(1) var<uniform> uLighting: LightingUniforms;

@group(1) @binding(0) var<uniform> globalModelMatrix: mat4x4f;
@group(1) @binding(1) var<uniform> chunkModelMatrix: mat4x4f;
@group(1) @binding(2) var colors: texture_2d<f32>;
@group(1) @binding(3) var textureSampler: sampler;


@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    let world: vec4f = globalModelMatrix * chunkModelMatrix * vec4f(in.position, 1.0);

    // Forward to fragment shader
    var out: VertexOutput;
    out.position = uMyUniforms.projectionMatrix * uMyUniforms.viewMatrix * world;
    
    let data: vec4<f32> = unpack4x8unorm(bitcast<u32>(in.data));

    // Pass voxel value
    out.voxelValue = data.x;

    // Ambient occlusion value [0, 1]
    out.ambientOcclusion = 1.0f - data.y * 256.0f/3.0f;

    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let uCoord = in.voxelValue * (254.0f/255.0f) + (0.5f/255.0f);
    let baseColor = textureSample(colors, textureSampler, vec2f(uCoord, 0.5)).rgb;
    return vec4f(baseColor - in.ambientOcclusion, 1.0);
}
)""