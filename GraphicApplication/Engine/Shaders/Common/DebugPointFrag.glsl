

layout(location = 0) in VertexData
{
    vec4 color;
    vec2 uv;

} FragmentIn;

layout(location = 0) out vec4 FragColor;

void main()
{
    float distSq = dot(FragmentIn.uv, FragmentIn.uv);

    if (distSq > 1.0)
    {
        discard;
    }

    FragColor = FragmentIn.color;
}

