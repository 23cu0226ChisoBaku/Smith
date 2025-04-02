// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMap.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップクラス

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMap.h"
#include "SmithSection.h"
#include "SmithRect.h"

#include "MLibrary.h"
#include "InvalidValues.h"

#include <limits>

///
/// @brief 内部使用構造体
///
namespace SmithMap::Private
{
  ///
  /// @brief 部屋の情報
  /// 
  struct RoomInfo
  {
    uint8 SectionIdx;
    FInt32Vector2 OriginCoord;

    bool operator==(const RoomInfo& other)
    {
      return (SectionIdx == other.SectionIdx) && (OriginCoord == other.OriginCoord);
    }
  };
}

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithMap実装
    ///
    // FSmithMap Implementation
    #pragma region FSmithMap Implementation
    class FSmithMap::MapImpl
    {
      //---------------------------------------
      /*
                　　　　エイリアス
      */
      //---------------------------------------
      private:
        using RoomInfo = typename SmithMap::Private::RoomInfo;

      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------       
      public:
        MapImpl()
          : m_sections{}
          , m_mapRect{}
          , m_row(0)
          , m_column(0)
          , m_sectionGap(0)
          , m_sectionWidth(0)
          , m_sectionHeight(0)
        { }
        ~MapImpl()
        {
          // c++17 structured binding
          // [key,value]
          for (auto& [sectionIdx, section] : m_sections)
          {
            section.Reset();
          }
          m_sections.Empty();
        }

      public:

        ///
        /// @brief  マップを生成する（セクションは均等で生成される）
        ///
        void GenerateMap(uint8 row, uint8 column, uint8 widthPerSection, uint8 heightPerSection, uint8 sectionGap, uint8 defaultValue)
        {
          // 入力値チェック
          ensure((row != 0) && (column != 0) && (widthPerSection != 0) && (heightPerSection != 0));
          ensure(StaticCast<int32>(row) * StaticCast<int32>(column) <= 256);
          ensure(StaticCast<int32>(widthPerSection) * StaticCast<int32>(column) + StaticCast<int32>((column + 1u) * sectionGap) <= 256);
          ensure(StaticCast<int32>(heightPerSection) * StaticCast<int32>(row) + StaticCast<int32>((row + 1u) * sectionGap) <= 256);

          // セクションギャップの大きさを計算
          const uint8 widthGap = (column + 1u) * sectionGap;
          const uint8 heightGap = (row + 1u) * sectionGap;
          const uint8 mapWidth = column * widthPerSection + widthGap;
          const uint8 mapHeight = row * heightPerSection + heightGap;

          // 例外安全
          // 一時的なマップ矩形作成
          FSmithRect tempRect{};
          tempRect.GenerateRect(mapWidth, mapHeight, defaultValue);
          // 一時的なセクションコンテナ作成
          TMap<uint8, TSharedPtr<FSmithSection>> tempSections;
          tempSections.Reserve(row * column);

          // セクションを作成
          for (uint8 y = 0u; y < row; ++y)
          {
            for (uint8 x = 0u; x < column; ++x)
            {
              const uint8 sectionIdx = y * column + x;
              const uint8 gapX = (x + 1u) * sectionGap;
              const uint8 gapY = (y + 1u) * sectionGap;
              const uint8 left = x * widthPerSection + gapX;
              const uint8 top = y * heightPerSection + gapY;
              const uint8 right = left + widthPerSection - 1u; 
              const uint8 bottom = top + heightPerSection - 1u;

              TSharedPtr<FSmithSection> section = ::MakeShared<FSmithSection>(sectionIdx);
              section->GenerateSection(left, top, right, bottom, defaultValue);

              tempSections.Emplace(sectionIdx, ::MoveTemp(section));
            }
          }

          // 例外安全のため最後に代入
          m_sections.Empty();
          m_sections.Reserve(row * column);
          m_sections = ::MoveTemp(tempSections);
          m_mapRect = ::MoveTemp(tempRect);
          m_row = row;
          m_column = column;
          m_sectionGap = sectionGap;
          m_sectionWidth = widthPerSection;
          m_sectionHeight = heightPerSection;
        }

        ///
        /// @brief  マップに部屋を生成する
        /// 
        void GenerateRoom(uint8 sectionIdx, uint8 roomMinWidth, uint8 roomMaxWidth, uint8 roomMinHeight, uint8 roomMaxHeight, uint8 defaultValue)
        { 
          // 入力値チェック   
          if (!m_sections.Contains(sectionIdx) || !m_sections[sectionIdx].IsValid())
          {
            MDebug::LogError(TEXT("Generate Room Error Occured. SectionIdx:") + FString::FromInt(sectionIdx));
            MDebug::LogError(TEXT("Exit Generate Rooms"));
            return;
          }

          const uint8 sectionWidth = m_sections[sectionIdx]->GetWidth();
          const uint8 sectionHeight = m_sections[sectionIdx]->GetHeight();

          // 入力値をチェック
          if ((roomMinWidth > sectionWidth || roomMaxWidth > sectionWidth)
              || (roomMinHeight > sectionHeight || roomMaxHeight > sectionHeight))
          {
            MDebug::LogError(TEXT("Can not create room at section ") + FString::FromInt(sectionIdx + 1));
            MDebug::LogError(TEXT("Index: ") + FString::FromInt(sectionIdx));
            MDebug::LogError(TEXT("Exit Generate Rooms"));
            return;
          }

          // 部屋を作成
          const uint8 roomWidth = StaticCast<uint8>(FMath::RandRange(roomMinWidth, roomMaxWidth)); 
          const uint8 roomHeight = StaticCast<uint8>(FMath::RandRange(roomMinHeight, roomMaxHeight));
          const uint8 roomMaxLeft = sectionWidth - roomWidth; 
          const uint8 roomMaxTop = sectionHeight - roomHeight;
          const uint8 roomLeft = StaticCast<uint8>(FMath::RandRange(0, roomMaxLeft));
          const uint8 roomTop = StaticCast<uint8>(FMath::RandRange(0, roomMaxTop));
          const uint8 roomRight = roomLeft + roomWidth - 1;
          const uint8 roomBottom = roomTop + roomHeight - 1;

          m_sections[sectionIdx]->GenerateRoom(roomLeft, roomTop, roomRight, roomBottom, defaultValue);

          // マップ矩形にデータを書き込む
          FSmithRect sectionRect = m_sections[sectionIdx]->GetSectionRect();
          const uint8 sectionRow = sectionIdx / m_column;
          const uint8 sectionColumn = sectionIdx % m_column;
          const uint8 xOffset = sectionColumn * (sectionWidth + m_sectionGap) + m_sectionGap;
          const uint8 yOffset = sectionRow * (sectionHeight + m_sectionGap) + m_sectionGap;

          for (uint8 y = 0u; y < sectionRect.GetHeight(); ++y)
          {
            for (uint8 x = 0u; x < sectionRect.GetWidth(); ++x)
            {
              const uint8 rectValue = sectionRect.GetRect(x, y);
              m_mapRect.SetRect(x + xOffset, y + yOffset, rectValue);
            }
          }      
        }

        ///
        /// @brief  生成した部屋を接続する
        ///
        void ConnectRooms(uint8 corridorData)
        {
          // マップ初期化される前にこの以降は実行しない
          const int32 sectionNum = m_sections.Num();
          if (sectionNum == 0)
          {
            MDebug::LogError("Generate room before connect");
            MDebug::LogError("Exit ConnectRooms");
            return;
          }

          TArray<RoomInfo> roomInfos;
          roomInfos.Reserve(sectionNum);

          // セクションにある部屋の情報を取得
          for (const auto& [sectionIdx, section] : m_sections)
          {
            // 部屋がなかったら飛ばす
            if (!section.IsValid() || !section->HasRoom())
            {
              continue;
            }
            
            // 部屋情報を取得
            const uint8 sectionRow = section->GetSectionIdx() / m_column;
            const uint8 sectionColumn = section->GetSectionIdx() % m_column;
            const uint8 leftGap = sectionColumn * (section->GetWidth() + m_sectionGap) + m_sectionGap;
            const uint8 topGap = sectionRow * (section->GetHeight() + m_sectionGap) + m_sectionGap;
            const int32 onMapRoomLeft = StaticCast<int32>(section->GetRoomLeft()) + StaticCast<int32>(leftGap);
            const int32 onMapRoomTop = StaticCast<int32>(section->GetRoomTop()) + StaticCast<int32>(topGap);
            roomInfos.Emplace(RoomInfo{ section->GetSectionIdx(), FInt32Vector2(onMapRoomLeft, onMapRoomTop)});
          }

          // 始点部屋をランダムに決める
          const int32 randomStartIdx = FMath::RandRange(0, roomInfos.Num() - 1);
          RoomInfo currentRoom = roomInfos[randomStartIdx];
          roomInfos.Remove(currentRoom);

          // 通路の座標を入れるコンテナ
          TArray<FUint32Vector2> corridorCoords;

          // 始点から一番近い部屋を探して、通路を繋げていく
          while (roomInfos.Num() > 0)
          {
            const RoomInfo closestRoom = findClosestRoomTo(roomInfos, currentRoom);
            roomInfos.Remove(closestRoom);
            appendCorridorIndex(corridorCoords, currentRoom, closestRoom);
            // 次の部屋から探す
            currentRoom = closestRoom;
          }

          // 通路状態をマップに書き込む
          for(const auto& corridorCoord : corridorCoords)
          {
            m_mapRect.SetRect(StaticCast<uint8>(corridorCoord.X), StaticCast<uint8>(corridorCoord.Y), corridorData);
          }
        }

        ///
        /// @brief  セクションの数を取得
        ///
        int32 GetSectionCount() const
        {
          return m_sections.Num();
        }

        ///
        /// @brief  セクションの行数を取得
        ///        
        uint8 GetRow() const
        {
          return m_row;
        }

        ///
        /// @brief  セクションの列数を取得
        ///
        uint8 GetColumn() const
        {
          return m_column;
        }

        ///
        /// @brief  マップの矩形を取得
        ///
        FSmithRect GetMapRect() const
        {
          return m_mapRect;
        }

        ///
        /// @brief  セクションを取得
        /// 
        FSmithSection* GetSection(uint8 rowIdx, uint8 columnIdx) const
        {
          // 入力値チェック
          if (rowIdx >= m_row || columnIdx >= m_column)
          {
            return nullptr;
          }

          // 初期化チェック
          const uint8 sectionIdx = rowIdx * m_column + columnIdx;
          if (!m_sections.Contains(sectionIdx))
          {
            return nullptr;
          }

          return m_sections[sectionIdx].Get();
        }

        ///
        /// @brief  座標でセクションを取得
        ///
        FSmithSection* GetSectionByCoord(uint8 x, uint8 y) const
        {
          // 初期化チェック
          if (m_sectionHeight == 0u || m_sectionWidth == 0u)
          {
            return nullptr;
          }

          // 座標がギャップの範囲内にあるか（あったらnullptr返し）
          if ((x % (m_sectionWidth + m_sectionGap)) < m_sectionGap 
              || (y % (m_sectionHeight + m_sectionGap)) < m_sectionGap)
          {
            return nullptr;
          }

          const uint8 sectionRow = y / (m_sectionHeight + m_sectionGap);
          const uint8 sectionColumn = x / (m_sectionWidth + m_sectionGap);
          return GetSection(sectionRow, sectionColumn);
        }

        /// 
        /// @brief  (N-1)列インデックスでN列目セクションの左座標を取得
        /// 
        uint8 GetSectionLeft(uint8 columnIdx) const
        {          
          // 入力値チェック（無効値255u）
          if (columnIdx >= m_column)
          {
            return InvalidValues::MAP_COORD_INVALID;
          }

          const uint8 sectionLeft = (columnIdx * m_sectionWidth) + ((columnIdx + 1) * m_sectionGap); 
          return sectionLeft;
        }
        /// 
        /// @brief  (N-1)行インデックスでN行目セクションの上座標を取得
        ///
        uint8 GetSectionTop(uint8 rowIdx) const
        {
          // 入力値チェック（無効値255u）
          if (rowIdx >= m_row)
          {
            return InvalidValues::MAP_COORD_INVALID;
          }

          const uint8 sectionTop = (rowIdx * m_sectionHeight) + ((rowIdx + 1) * m_sectionGap);  
          return sectionTop;
        }

        ///
        /// @brief  マップの横幅を取得
        ///
        uint8 GetMapWidth() const
        {
          return m_mapRect.GetWidth();
        }

        ///
        /// @brief  マップの高さを取得
        ///
        uint8 GetMapHeight() const
        {
          return m_mapRect.GetHeight();
        }

      private:
        ///
        /// @brief              一番近い部屋を探す
        /// @param rooms        残りの部屋
        /// @param currentRoom  現在の部屋
        /// @return             見つけた部屋
        ///
        RoomInfo findClosestRoomTo(const TArray<RoomInfo>& rooms, const RoomInfo& currentRoom)
        {
          FInt32Vector2 closest = currentRoom.OriginCoord;
          uint8 closestRoomSectionIdx = currentRoom.SectionIdx;
          float minDistance = std::numeric_limits<float>::max();

          // 今の部屋に一番近い部屋を探す
          for (const auto& room : rooms)
          {
            // 距離を計算
            const int32 distanceSquaredX = FMath::Square(currentRoom.OriginCoord.X - room.OriginCoord.X);
            const int32 distanceSquaredY = FMath::Square(currentRoom.OriginCoord.Y - room.OriginCoord.Y);
            const float distance = FMath::Sqrt(StaticCast<float>(distanceSquaredX + distanceSquaredY));

            if (distance < minDistance)
            {
              minDistance = distance;
              closest = room.OriginCoord;
              closestRoomSectionIdx = room.SectionIdx;
            }
          }
          return RoomInfo{closestRoomSectionIdx, closest};
        }
        ///
        /// @brief              二つの部屋を繋げていく道路座標を取得
        /// @param outCoordArr  通路座標を入れるコンテナ（Out）
        /// @param roomFrom     始点部屋
        /// @param roomTo       終点部屋
        ///
        void appendCorridorIndex(TArray<FUint32Vector2>& outCoordArr, const RoomInfo& roomFrom, const RoomInfo& roomTo)
        {
          const uint8 fromRight = roomFrom.OriginCoord.X + m_sections[roomFrom.SectionIdx]->GetRoomWidth() - 1;
          const uint8 fromBottom = roomFrom.OriginCoord.Y + m_sections[roomFrom.SectionIdx]->GetRoomHeight() - 1;
          const uint8 toRight = roomTo.OriginCoord.X + m_sections[roomTo.SectionIdx]->GetRoomWidth() - 1;
          const uint8 toBottom = roomTo.OriginCoord.Y + m_sections[roomTo.SectionIdx]->GetRoomHeight() - 1;

          uint8 fromRandX = FMath::RandRange(roomFrom.OriginCoord.X + 1, fromRight - 1);
          uint8 fromRandY = FMath::RandRange(roomFrom.OriginCoord.Y + 1, fromBottom - 1);
          const uint8 toRandX = FMath::RandRange(roomTo.OriginCoord.X + 1, toRight - 1);
          const uint8 toRandY = FMath::RandRange(roomTo.OriginCoord.Y + 1, toBottom - 1);

          // XからもしくはYから道を伸ばすかをランダムに決める
            // Xから
            if (FMath::Abs(roomFrom.OriginCoord.X - roomTo.OriginCoord.X) > FMath::Abs(roomFrom.OriginCoord.Y - roomTo.OriginCoord.Y))
            {
              const uint8 centerX = roomFrom.OriginCoord.X > roomTo.OriginCoord.X ?
                                    StaticCast<uint8>(StaticCast<int32>(toRight + roomFrom.OriginCoord.X) / 2) :
                                    StaticCast<uint8>(StaticCast<int32>(fromRight + roomTo.OriginCoord.X) / 2) ;
              while (fromRandX != centerX)
              {
                if (fromRandX < toRandX)
                {
                  ++fromRandX;         
                }
                else if (fromRandX > toRandX)
                {
                  --fromRandX;
                }
                outCoordArr.Emplace(FUint32Vector2(fromRandX, fromRandY));
              }

              while (fromRandY != toRandY)
              {
                if (fromRandY < toRandY)
                {
                  ++fromRandY;
                }
                else if (fromRandY > toRandY)
                {
                  --fromRandY;
                }
                outCoordArr.Emplace(FUint32Vector2(centerX, fromRandY));
              }

              while (fromRandX != toRandX)
              {
                if (fromRandX < toRandX)
                {
                  ++fromRandX;         
                }
                else if (fromRandX > toRandX)
                {
                  --fromRandX;
                }
                outCoordArr.Emplace(FUint32Vector2(fromRandX, toRandY));
              }
            }
            // Yから
            else
            {
              const uint8 centerY = roomFrom.OriginCoord.Y > roomTo.OriginCoord.Y ?
                                    StaticCast<uint8>(StaticCast<int32>(toBottom + roomFrom.OriginCoord.Y) / 2) :
                                    StaticCast<uint8>(StaticCast<int32>(fromBottom + roomTo.OriginCoord.Y) / 2);

              while (fromRandY != centerY)
              {
                if (fromRandY < toRandY)
                {
                  ++fromRandY;
                }
                else if (fromRandY > toRandY)
                {
                  --fromRandY;
                }
                outCoordArr.Emplace(FUint32Vector2(fromRandX, fromRandY));
              }

              while (fromRandX != toRandX)
              {
                if (fromRandX < toRandX)
                {
                  ++fromRandX;         
                }
                else if (fromRandX > toRandX)
                {
                  --fromRandX;
                }
                outCoordArr.Emplace(FUint32Vector2(fromRandX, centerY));
              }

              while (fromRandY != toRandY)
              {
                if (fromRandY < toRandY)
                {
                  ++fromRandY;
                }
                else if (fromRandY > toRandY)
                {
                  --fromRandY;
                }
                outCoordArr.Emplace(FUint32Vector2(fromRandX, fromRandY));
              }
            }
          }
      private:
        TMap<uint8, TSharedPtr<FSmithSection>> m_sections;
        FSmithRect m_mapRect;
        uint8 m_row;
        uint8 m_column;
        uint8 m_sectionGap;
        uint8 m_sectionWidth;
        uint8 m_sectionHeight;

    };
    #pragma endregion FSmithMap Implementation
    // end of FSmithMap Implementation
    
    // FSmithMap Interface
    #pragma region FSmithMap Interface
    FSmithMap::FSmithMap()
      : m_pImpl(::MakeUnique<MapImpl>())
    { }

    FSmithMap::~FSmithMap()
    {
      m_pImpl.Reset();
    }

    void FSmithMap::GenerateMap(uint8 row, uint8 column, uint8 widthPerSection, uint8 heightPerSection, uint8 sectionGap, uint8 defaultValue)
    {
      m_pImpl->GenerateMap(row, column, widthPerSection, heightPerSection, sectionGap, defaultValue);
    }

    void FSmithMap::GenerateRoom(uint8 sectionIdx, uint8 roomMinWidth, uint8 roomMaxWidth, uint8 roomMinHeight, uint8 roomMaxHeight, uint8 defaultValue)
    {
      m_pImpl->GenerateRoom(sectionIdx, roomMinWidth, roomMaxWidth, roomMinHeight, roomMaxHeight, defaultValue);
    }

    void FSmithMap::ConnectRooms(uint8 corridorData)
    {
      m_pImpl->ConnectRooms(corridorData);
    }

    int32 FSmithMap::GetSectionCount() const
    {
      return m_pImpl->GetSectionCount();
    }

    uint8 FSmithMap::GetRow() const
    {
      return m_pImpl->GetRow();
    }

    uint8 FSmithMap::GetColumn() const
    {
      return m_pImpl->GetColumn();
    }

    FSmithRect FSmithMap::GetMap() const
    {
      return m_pImpl->GetMapRect();
    }

    FSmithSection* FSmithMap::GetSection(uint8 rowIdx, uint8 columnIdx) const
    {
      return m_pImpl->GetSection(rowIdx, columnIdx);
    }

    FSmithSection* FSmithMap::GetSectionByCoord(uint8 x, uint8 y) const
    {
      return m_pImpl->GetSectionByCoord(x, y);
    }

    uint8 FSmithMap::GetSectionLeft(uint8 columnIdx) const
    {
      return m_pImpl->GetSectionLeft(columnIdx);
    }

    uint8 FSmithMap::GetSectionTop(uint8 rowIdx) const
    {
      return m_pImpl->GetSectionTop(rowIdx);
    }

    uint8 FSmithMap::GetMapWidth() const
    {
      return m_pImpl->GetMapWidth();
    }

    uint8 FSmithMap::GetMapHeight() const
    {
      return m_pImpl->GetMapHeight();
    }
    #pragma endregion FSmithMap Interface
    // end of FSmithMap Interface
  }
}
