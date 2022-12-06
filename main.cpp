#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

template <typename Word>
std::ostream& writeWord( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
{
  for (; size; --size, value >>= 8)
    outs.put( static_cast <char> (value & 0xFF) );
  return outs;
}

void generateFile(std::ofstream& f, const float hz, const float lengthSeconds)
{
    // Prepare the format
    const int sampleRate = 48000;
    const int channels = 1;
    const int bitsPerSample = 24;
    
    f << "RIFF----WAVEfmt ";
    writeWord(f, 16, 4);
    writeWord(f, 1, 2);
    writeWord(f, channels, 2);
    writeWord(f, sampleRate, 4);
    writeWord(f, sampleRate * bitsPerSample * channels, 4);
    writeWord(f, 2, 2);
    writeWord(f, bitsPerSample, 2);

    size_t data_chunk_pos = f.tellp();
    f << "data----";

    // Start writing samples
    long maxSampleCount = sampleRate * lengthSeconds;
    long sampleCount = 0;
    double wavelengthInSamples = sampleRate / hz;
    const int maxAmplitude = 8388607;
    while (sampleCount < maxSampleCount)
    {
        for(int i = 0; i<wavelengthInSamples; i++)
        {
            int32_t sample;
            sample = maxAmplitude * sin(2 * M_PI * (i / wavelengthInSamples));
            writeWord(f, sample, 3);
            sampleCount++;
        }
    }

    // Finalize header
    size_t file_length = f.tellp();
    f.seekp( data_chunk_pos + 4 );
    writeWord( f, file_length - data_chunk_pos + 8 );

    f.seekp( 0 + 4 );
    writeWord( f, file_length - 8, 4 );
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <length in seconds> <frequency>" << std::endl;
        return 1;
    }
    
    const float length = atof(argv[1]);
    const float frequency = atof(argv[2]);

    const auto fileName = "audio.wav";
    
    auto exists = std::ifstream(fileName).good();
    if (exists)
    {
        int status = std::remove(fileName);
        if(status != 0)
        {
            std::cerr << "Error when deleting the file." << std::endl;
            return 1;
        }
    }
    
    std::ofstream f(fileName, std::ios::binary );
    f.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    try
    {
        generateFile(f, frequency, length);
    }
    catch (std::ofstream::failure &e) 
    {
        std::cerr << "Error when generating the file: " << e.what() << std::endl;
    }
}