#include "ICAnalysis/ElectronConversionCalculator/plugins/ICElectronConversionFromPatCalculator.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

ICElectronConversionFromPatCalculator::ICElectronConversionFromPatCalculator(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")) {
  produces<edm::ValueMap<bool> >();
}

ICElectronConversionFromPatCalculator::~ICElectronConversionFromPatCalculator() {}

void ICElectronConversionFromPatCalculator::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<bool> > product(new edm::ValueMap<bool>());
  edm::Handle<pat::ElectronCollection> elecs_handle;
  event.getByLabel(input_, elecs_handle);
  
  std::vector<bool> values(elecs_handle->size(), 0.);
  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    pat::Electron const& src = elecs_handle->at(i);
    values[i] = src.passConversionVeto();
  }

  edm::ValueMap<bool>::Filler filler(*product);
  filler.insert(elecs_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);
}

void ICElectronConversionFromPatCalculator::beginJob() {}

void ICElectronConversionFromPatCalculator::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronConversionFromPatCalculator);
