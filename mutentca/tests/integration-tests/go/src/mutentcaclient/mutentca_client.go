package mutentcaclient

import "gen/api/models"

// MutentCAClientInterface is the client interface
type MutentCAClientInterface interface {
	Login(username string, password string) error

	GetCAVersion() (*models.Version, error)
	GetRootCA() (*models.CACertificates, error)

	CreateIntermediateCA(caId string, parentCAId string, country []string, state []string, locality []string, orgUnit []string, policy string, startTime string, endTime string) (*models.CACertificates, error)
	GetIntermediateCA(caId string) (*models.CACertificates, error)
	RevokeIntermediateCA(caId string) error

	GetRootCACRL() (*models.CRL, error)
	GetIntermediateCACRL(caId string) (*models.CRL, error)

	GetRootCASignedCert(csr string, startTime string, endTime string, signatureAlgo string) (*models.Certificate, error)
	RevokeRootCASignedCert(cert string) error
	GetIntermediateCASignedCert(caId string, csr string, startTime string, endTime string, signatureAlgo string) (*models.Certificate, error)
	RevokeIntermediateCASignedCert(caId string, cert string) error
}
