#version 330 core

in vec2 vTexCoords;
uniform sampler2D texUnit;

uniform float texSize;

out vec4 out_Color;

//LP-filter with the specified amount of rows, currently uniform.
vec4 lpfilter(int rows){
    vec4 result = vec4(0.0);
    float offset = 1.0 / 1024;
    float count = 0;
    vec2 tc = vTexCoords;

    tc.y = tc.y - ((rows-1)/2.0)*offset;
    for (int i = 0; i < rows; i++){
        tc.x = vTexCoords.x - ((rows-1)/2.0)*offset;

        for (int j = 0; j < rows; j++){
            //check if tc is inside bounds
            if(!(tc.x > 1.0 || tc.x < 0.0 ||
               tc.y > 1.0 || tc.y < 0.0)){
                result += texture(texUnit, tc) ;
                count++;
            }
            tc.x = tc.x + offset;
        }
        tc.y = tc.y + offset;
    }

    return result * 1/(count);
}

void main(void)
{

		out_Color = lpfilter(10);
}
