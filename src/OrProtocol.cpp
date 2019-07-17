#include "OrProtocol.hpp"


OrProtocol::OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, 
                       int i_known /*= 0*/)
                       :
                       sigma_prots_(sigma_protocols), 
                       i_known_(i_known)
{
    num_prots_ = sigma_protocols.size();

    assert(i_known_ >= 0 && i_known_ < num_prots_);

    for (int i = 1; i < num_prots_; i++) {
        assert(sigma_prots_[i]->challengeSize() == 
               sigma_prots_[i - 1]->challengeSize());
    }
}


void OrProtocol::setKnown(int i_known)
{
    i_known_ = i_known;
}


void OrProtocol::generateCommitment()
{
    for (int i = 0; i < num_prots_; i++) {
        if (i == i_known_)
            sigma_prots_[i]->generateCommitment();
        else {
            sigma_prots_[i]->generateChallenge();
            sigma_prots_[i]->generateSimulation();
        }
    }
}


void OrProtocol::generateChallenge(CryptoPP::Integer e)
{
    assert(i_known_ >= 0);

    total_e_ = 0;
    for (int i = 0; i < num_prots_; i++) {
        if (i != i_known_)
            total_e_ += sigma_prots_[i]->challenge();
    }
    assert(e > total_e_);
    e_ = e;
}


void OrProtocol::generateResponse()
{
    assert(e_ > 0);

    CryptoPP::Integer e_known = e_ - total_e_;
    sigma_prots_[i_known_]->generateChallenge(&e_known);
    sigma_prots_[i_known_]->generateResponse();
}


bool OrProtocol::verify()
{
    assert(e_ > 0);

    for (SigmaProtocol* prot : sigma_prots_) {
        if (prot->verify() == false)
            return false;
    }
    if (e_ != total_e_ + sigma_prots_[i_known_]->challenge())
        return false;

    return true;
}


CryptoPP::Integer OrProtocol::challengeSize()
{ 
    return sigma_prots_[0]->challengeSize();
}


std::string OrProtocol::getHashData() 
{
    std::string ret;
    for (int i = 0; i < num_prots_; i++)
        ret += sigma_prots_[i]->getHashData();
    return ret;
}


OrTranscript OrProtocol::generateNIZKP()
{
    generateCommitment();
    auto hash_challenge = GenHashChallenge(getHashData(), challengeSize());
    hash_challenge += (num_prots_ - 1) * challengeSize();
    generateChallenge(hash_challenge);
    generateResponse();

    auto* transcripts = new Transcript[num_prots_];
    for (int i = 0; i < num_prots_; i++) {
        transcripts[i] = sigma_prots_[i]->getTranscript();
    }
    auto ret = OrTranscript(transcripts, num_prots_, e_);
    delete [] transcripts;
    return ret;
}


bool OrProtocol::verifyNIZKP(const OrTranscript& or_nizkp)
{
    assert(num_prots_ == or_nizkp.numProts());

    for (int i = 0; i < num_prots_; i++) {
        sigma_prots_[i]->setTranscript(or_nizkp.transcript(i));
        if (sigma_prots_[i]->verify() == false)
            return false;
    }
    
    auto hash_e = GenHashChallenge(getHashData(), challengeSize());
    hash_e += (num_prots_ - 1) * challengeSize();

    CryptoPP::Integer total_challenge = 0;
    for (SigmaProtocol* prot : sigma_prots_)
        total_challenge += prot->challenge();
        
    if (hash_e != or_nizkp.e())
        return false;
    if (total_challenge != hash_e)
        return false;

    return true;
}