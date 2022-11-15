// 파란 블로그 참조
// 인덱스는 처음 생성할때 지정  

// vibuffer를 상속받는 트레일 버퍼.cpp
// Tick

// 용준 팁: UV y에 * 0.2f 곱해서 y가 촘촘하게 하면 보기 좋다.
// 알파값 넣으면 구려짐

if (m_bTrailOn)
   {
    //m_iVtxCount: 현재 개수
    //m_iNumVertices 그대로
      if (m_iVtxCount < m_iNumVertices)
      {
         D3D11_MAPPED_SUBRESOURCE      SubResource;

         DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

         // 짝수가 아래쪽 0, 2, 4, 6
         // 홀수가 위쪽 1, 3, 5, 6
         _float4 vHandPos = m_vLocalSwordLow;

         if (!m_pRefBone)
         {
            _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
            vHandPos = vHandPos.MultiplyCoord(matWorld);
         }
         else
         {
            // 뼈에서 행렬 가져오고
            _matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
            matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
            _float4x4 matBone;
            XMStoreFloat4x4(&matBone, matRightHand);
            _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

            vHandPos = vHandPos.MultiplyCoord(matBone);
            // 월드를 반영한다. 뼈의 움직임 + 월드 상의 트랜스폼 반영
            // 만약 트레일 버퍼가 중간에 꺼지고 플레이어가 움직이면 어색해지지만,
            // 트레일버퍼 포지션에 월드를 넣지 않고 객체의 월드행렬에 던지고 버텍스 셰이더에서 곱해줘도 된다.
            vHandPos = vHandPos.MultiplyCoord(matWorld);
         }


         ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.x = vHandPos.x;
         ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.y = vHandPos.y;
         ((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.z = vHandPos.z;


         //위쪽
         vHandPos = m_vLocalSwordHigh;
         if (!m_pRefBone)
         {
            _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
            vHandPos = vHandPos.MultiplyCoord(matWorld);
         }
         else
         {
            _matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
            matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
            _float4x4 matBone;
            XMStoreFloat4x4(&matBone, matRightHand);
            _float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

            vHandPos = vHandPos.MultiplyCoord(matBone);
            vHandPos = vHandPos.MultiplyCoord(matWorld);
         }

         ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.x = vHandPos.x;
         ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.y = vHandPos.y;
         ((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.z = vHandPos.z;

         m_iVtxCount += 2;

         if (m_iVtxCount > m_iNumVertices)
            m_iVtxCount = m_iNumVertices;

         for (_uint i = 0; i < m_iVtxCount; i += 2)
         {
            ((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 1.f };
            ((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 0.f };
         }

         DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

      }
      else
      {
         //버텍스사이즈 초과함
         _uint   iRemoveCount = 2;

         if (iRemoveCount % 2 > 0)
         {
            iRemoveCount -= 1;
         }

         if (iRemoveCount < 2)
            iRemoveCount = 2;

         if (m_iVtxCount >= 2)
            m_iVtxCount -= iRemoveCount;


         D3D11_MAPPED_SUBRESOURCE      SubResource;
         DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

        // 확줄어들게 하고 싶어서 4보다 적을때로 바꿈. 
         if (m_iVtxCount <= m_iNumVertices / 4) 
         {
            // 초과하면 기존을 지우고(한쪽으로 축소)
            for (_uint i = 2; i < m_iNumVertices; i += 2)
            {
               ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
               ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
            }
            m_iVtxCount = 0;

         }

        // 초과하면 모두 한칸씩 앞으로 땡긴다.
         for (_uint i = 0; i < m_iVtxCount; i += 2)
         {
            ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
            ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
         }
         DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
      }
   }
else
   {
      // 트레일 버퍼 꺼지면 천천히 지움
      _uint   iRemoveCount = 2;

      if (iRemoveCount % 2 > 0)
      {
         iRemoveCount -= 1;
      }

      if (iRemoveCount < 2)
         iRemoveCount = 2;

      if (m_iVtxCount >= 2)
         m_iVtxCount -= iRemoveCount;


      D3D11_MAPPED_SUBRESOURCE      SubResource;
      DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

      if (m_iVtxCount <= m_iNumVertices / 4)
      {
         for (_uint i = 2; i < m_iNumVertices; i += 2)
         {
            ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
            ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
         }
         m_iVtxCount = 0;

      }


      for (_uint i = 0; i < m_iVtxCount; i += 2)
      {
         ((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
         ((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
      }
      DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
   }