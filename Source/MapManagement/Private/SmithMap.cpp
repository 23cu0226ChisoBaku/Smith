// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMap.h"
#include "SmithSection.h"
#include "SmithRect.h"

#include "MLibrary.h"
#include "InvalidValues.h"
#include "MLibrary.h"

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
        { }
        ~MapImpl()
        {
          for (auto& sectionPair : m_sections)
          {
            sectionPair.Value.Reset();
          }
          m_sections.Empty();
        }

      public:
        void GenerateMap(uint8 row, uint8 column, uint8 widthPerSection, uint8 heightPerSection, uint8 sectionGap, uint8 defaultValue)
        {
          // 入力値チェック
          check((row != 0) && (column != 0) && (widthPerSection != 0) && (heightPerSection != 0));
          check(StaticCast<int32>(row) * StaticCast<int32>(column) <= 256);
          check(StaticCast<int32>(widthPerSection) * StaticCast<int32>(column) + StaticCast<int32>((column + 1) * sectionGap) <= 256);
          check(StaticCast<int32>(heightPerSection) * StaticCast<int32>(row) + StaticCast<int32>((row + 1) * sectionGap) <= 256);

          // ギャップを設ける
          const uint8 widthGap = (column + 1) * sectionGap;
          const uint8 heightGap = (row + 1) * sectionGap;

          const uint8 mapWidth = column * widthPerSection + widthGap;
          const uint8 mapHeight = row * heightPerSection + heightGap;

          // 一時的なマップ矩形作成
          FSmithRect tempRect{};
          tempRect.GenerateRect(mapWidth, mapHeight, defaultValue);
          // 一時的なセクションコンテナ作成
          TMap<uint8, TUniquePtr<FSmithSection>> tempSections;
          tempSections.Reserve(row * column);

          // セクションを作成
          for (uint8 y = 0; y < row; ++y)
          {
            for (uint8 x = 0; x < column; ++x)
            {
              const uint8 sectionIdx = y * column + x;

              const uint8 gapX = (x + 1) * sectionGap;
              const uint8 gapY = (y + 1) * sectionGap;
              const uint8 left = x * widthPerSection + gapX;
              const uint8 top = y * heightPerSection + gapY;
              const uint8 right = left + widthPerSection - 1; 
              const uint8 bottom = top + heightPerSection - 1;

              TUniquePtr<FSmithSection> section = ::MakeUnique<FSmithSection>(sectionIdx);
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

        void GenerateRoom(uint8 sectionIdx, uint8 roomMinWidth, uint8 roomMaxWidth, uint8 roomMinHeight, uint8 roomMaxHeight, uint8 defaultValue)
        { 
          // 入力値チェック   
          if (!m_sections.Contains(sectionIdx))
          {
            MDebug::LogError(TEXT("Section Index Error: Can not get section by Index: ") + FString::FromInt(sectionIdx));
            MDebug::LogError(TEXT("Exit Generate Rooms"));
            return;
          }
          if (!m_sections[sectionIdx].IsValid())
          {
            MDebug::LogError(TEXT("Section Invalid. Index: ") + FString::FromInt(sectionIdx));
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
                return;
              }

          // 部屋を作成
          const uint8 roomWidth = StaticCast<uint8>(FMath::RandRange(roomMinWidth, roomMaxWidth)); 
          const uint8 roomHeight = StaticCast<uint8>(FMath::RandRange(roomMinHeight, roomMaxHeight));
          const uint8 roomMaxLeft = sectionWidth - roomWidth; 
          const uint8 roomMaxTop = sectionHeight - roomHeight;
          const uint8 left = StaticCast<uint8>(FMath::RandRange(0, roomMaxLeft));
          const uint8 top = StaticCast<uint8>(FMath::RandRange(0, roomMaxTop));
          const uint8 right = left + roomWidth - 1;
          const uint8 bottom = top + roomHeight - 1;

          m_sections[sectionIdx]->GenerateRoom(left, top, right, bottom, defaultValue);

          // TODO マップ矩形にデータを書き込む
          FSmithRect sectionRect = m_sections[sectionIdx]->GetSectionRect();
          const uint8 sectionRow = sectionIdx / m_column;
          const uint8 sectionColumn = sectionIdx % m_column;
          const uint8 xOffset = sectionColumn * (m_sections[sectionIdx]->GetWidth() + m_sectionGap) + m_sectionGap;
          const uint8 yOffset = sectionRow * (m_sections[sectionIdx]->GetHeight() + m_sectionGap) + m_sectionGap;

          for (int y = 0; y < sectionRect.GetHeight(); ++y)
          {
            for (int x = 0; x < sectionRect.GetWidth(); ++x)
            {
              m_mapRect.SetRect(x + xOffset, y + yOffset, sectionRect.GetRect(x, y));
            }
          }      
        }
        void ConnectRooms(uint8 corridorData)
        {
          // 内部状態チェック
          if (m_sections.Num() == 0)
          {
            MDebug::LogError("Generate room before connect!!!");
            return;
          }

          TArray<RoomInfo> roomInfos;
          roomInfos.Reserve(m_sections.Num());

          // セクションにある部屋の情報を取得
          for (int32 i = 0; i < m_sections.Num(); ++i)
          {
            // 部屋がなかったら飛ばす
            if (!m_sections[i].IsValid() || !m_sections[i]->HasRoom())
            {
              continue;
            }
            
            const uint8 sectionRow = m_sections[i]->GetSectionIdx() / m_column;
            const uint8 sectionColumn = m_sections[i]->GetSectionIdx() % m_column;
            const uint8 leftOffset = sectionColumn * (m_sections[i]->GetWidth() + m_sectionGap) + m_sectionGap;
            const uint8 topOffset = sectionRow * (m_sections[i]->GetHeight() + m_sectionGap) + m_sectionGap;

            const int32 left = StaticCast<int32>(m_sections[i]->GetRoomLeft()) + StaticCast<int32>(leftOffset);
            const int32 top = StaticCast<int32>(m_sections[i]->GetRoomTop()) + StaticCast<int32>(topOffset);
            roomInfos.Emplace(RoomInfo{ m_sections[i]->GetSectionIdx(), FInt32Vector2(left, top)});
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
            RoomInfo closestRoom = findClosestRoomTo(roomInfos, currentRoom);
            roomInfos.Remove(closestRoom);

            appendCorridorIndex(corridorCoords, currentRoom, closestRoom);
            currentRoom = closestRoom;
          }

          // 通路状態をマップに書き込む
          for(const auto& corridorCoord : corridorCoords)
          {
            m_mapRect.SetRect(StaticCast<uint8>(corridorCoord.X), StaticCast<uint8>(corridorCoord.Y), corridorData);
          }
        }
        int32 GetSectionCount() const
        {
          return m_sections.Num();
        }
        uint8 GetRow() const
        {
          return m_row;
        }
        uint8 GetColumn() const
        {
          return m_column;
        }
        FSmithRect GetMapRect() const
        {
          return m_mapRect;
        }
        FSmithSection* GetSection(uint8 rowIdx, uint8 columnIdx) const
        {
          // 入力値チェック
          if (rowIdx >= m_row || columnIdx >= m_column)
          {
            return nullptr;
          }

          const uint8 sectionIdx = rowIdx * m_column + columnIdx;

          // 入力値チェック
          if (!m_sections.Contains(sectionIdx))
          {
            return nullptr;
          }

          return m_sections[sectionIdx].Get();
        }

        FSmithSection* GetSectionByCoord(uint8 x, uint8 y) const
        {
          // 入力値チェック
          if (x >= m_mapRect.GetWidth() || y >= m_mapRect.GetHeight())
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
          const uint8 sectionIdx = sectionRow * m_column + sectionColumn;

          // セクションIDが存在しない場合
          if (!m_sections.Contains(sectionIdx))
          {
            return nullptr;
          }

          return m_sections[sectionIdx].Get();
        }

        uint8 GetSectionLeft(uint8 columnIdx) const
        {          
          // 入力値チェック（無効値255u）
          if (columnIdx >= m_column)
          {
            return 0xffu;
          }

          const uint8 sectionLeft = columnIdx * m_sectionWidth + (columnIdx + 1) * m_sectionGap; 
          return sectionLeft;
        }
        // TODO
        uint8 GetSectionTop(uint8 rowIdx) const
        {
          // 入力値チェック（無効値255u）
          if (rowIdx >= m_row)
          {
            return 0xffu;
          }

          const uint8 sectionTop = rowIdx * m_sectionHeight + (rowIdx + 1) * m_sectionGap;  
          return sectionTop;
        }
        uint8 GetMapWidth() const
        {
          return m_mapRect.GetWidth();
        }
        uint8 GetMapHeight() const
        {
          return m_mapRect.GetHeight();
        }
      // TODO プライベート関数にする意味がない
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

          uint8 fromRandX = FMath::RandRange(roomFrom.OriginCoord.X, fromRight);
          uint8 fromRandY = FMath::RandRange(roomFrom.OriginCoord.Y, fromBottom);
          const uint8 toRandX = FMath::RandRange(roomTo.OriginCoord.X, toRight);
          const uint8 toRandY = FMath::RandRange(roomTo.OriginCoord.Y, toBottom);

          // XからもしくはYから道を伸ばすかをランダムに決める
          switch(FMath::RandRange(0,1))
          {
            // Xから
            case 0:
            {
              // 曲がる所の座標追加
              outCoordArr.Emplace(FUint32Vector2(toRandX, fromRandY));
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
                outCoordArr.Emplace(FUint32Vector2(fromRandX, fromRandY));
              }

            }
            break;

            // Yから
            case 1:
            {
              // 曲がる所の座標追加
              outCoordArr.Emplace(FUint32Vector2(fromRandX, toRandY));
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
                outCoordArr.Emplace(FUint32Vector2(fromRandX, fromRandY));
              }
            }
            break;
          }
        }
      private:
        TMap<uint8, TUniquePtr<FSmithSection>> m_sections;
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
