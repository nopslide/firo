#include "../schnorr_proof.h"
#include "../schnorr_prover.h"
#include "../schnorr_verifier.h"

#include <boost/test/unit_test.hpp>

namespace lelantus {

class SchnorrProofTests {
public:
    SchnorrProofTests()
    {
        g.randomize();
        h.randomize();
        P.randomize();
        T.randomize();
    }

public:
    GroupElement g, h;
    Scalar P, T;
};

BOOST_FIXTURE_TEST_SUITE(lelantus_schnorr_proof_tests, SchnorrProofTests)

BOOST_AUTO_TEST_CASE(serialization)
{
    SchnorrProver<Scalar, GroupElement> prover(g, h);
    SchnorrProof<Scalar, GroupElement> proof;
    prover.proof(P, T, proof);

    std::vector<unsigned char> buffer;
    buffer.resize(proof.memoryRequired());

    proof.serialize(buffer.data());

    SchnorrProof<Scalar, GroupElement> deserialized;
    deserialized.deserialize(buffer.data());

    BOOST_CHECK(proof.u == deserialized.u);
    BOOST_CHECK(proof.P1 == deserialized.P1);
    BOOST_CHECK(proof.T1 == deserialized.T1);
}

BOOST_AUTO_TEST_CASE(prove_verify)
{
    GroupElement y = LelantusPrimitives<Scalar, GroupElement>::commit(g, P, h, T);

    SchnorrProver<Scalar, GroupElement> prover(g, h);
    SchnorrProof<Scalar, GroupElement> proof;
    prover.proof(P, T, proof);

    SchnorrVerifier<Scalar, GroupElement> verifier(g, h);
    BOOST_CHECK(verifier.verify(y ,proof));
}

BOOST_AUTO_TEST_CASE(fake_prove_not_verify)
{
    GroupElement y;
    y.randomize();

    SchnorrProver<Scalar, GroupElement> prover(g, h);
    SchnorrProof<Scalar, GroupElement> proof;
    prover.proof(P, T, proof);

    SchnorrVerifier<Scalar, GroupElement> verifier(g, h);
    BOOST_CHECK(!verifier.verify(y ,proof));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace lelantus