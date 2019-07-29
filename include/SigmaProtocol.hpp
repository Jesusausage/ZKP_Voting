#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"
#include "Transcript.hpp"
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <assert.h>


class SigmaProtocol {
public:
    SigmaProtocol(const ECGroup& ecg, const CryptoPP::ECPPoint& generator);
    virtual ~SigmaProtocol() {}

    virtual void generateCommitment() = 0;
    void generateChallenge(CryptoPP::Integer* e = nullptr);
    virtual void generateResponse() = 0;
    virtual bool verify() = 0;
    virtual void generateSimulation() = 0;
    
    inline const CryptoPP::Integer& challenge() const 
        { return transcript_.challenge(); }
    inline const CryptoPP::Integer& challengeSize() const
        { return order_; }
    virtual std::string getHashData() = 0;

    inline const Transcript& getTranscript() const
        { return transcript_; }
    inline void setTranscript(const Transcript& transcript)
        { transcript_ = transcript; }

    Transcript generateNIZKP();
    bool verifyNIZKP(const Transcript& nizkp);

protected:    
    const CryptoPP::ECP& curve_;
    const CryptoPP::Integer& order_;
    const CryptoPP::ECPPoint& gen_;

    CryptoPP::Integer commitment_seed_;
    Transcript transcript_;
};


CryptoPP::Integer GenHashChallenge(std::string hash_data,
                                   const CryptoPP::Integer& challenge_max);


#endif