// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModelPresenter.h"

#include "UObject/WeakInterfacePtr.h"
#include "SmithMapDataModel.h"
#include "ISmithMapModelRequester.h"

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMapModelPresenter::PresenterImpl
    {
      public:

        PresenterImpl(TSharedPtr<FSmithMapDataModel> MapData, ISmithMapModelRequester* MapModelRequester)
          : m_mapData(MapData)
          , m_mapModelRequester(MapModelRequester)
        {
          check(m_mapData.IsValid());
          check(m_mapModelRequester.IsValid());
        }
        
        ~PresenterImpl()
        {
          m_mapData.Reset();
          m_mapModelRequester.Reset();
        }

      private:

        TSharedPtr<FSmithMapDataModel> m_mapData;
        TWeakInterfacePtr<ISmithMapModelRequester> m_mapModelRequester;
    };

    FSmithMapModelPresenter::FSmithMapModelPresenter(TSharedPtr<FSmithMapDataModel> MapData, ISmithMapModelRequester* MapModelRequester)
      : m_pimpl(::MakeUnique<PresenterImpl>(MapData, MapModelRequester))
    { }

    FSmithMapModelPresenter::~FSmithMapModelPresenter()
    {
      m_pimpl.Reset();
    }
  }
}