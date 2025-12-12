#pragma once
namespace UITimeOD
{
	const CString UIXAML_TIMEOD_BACKIMAGE    
		(
		_T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>") 
		_T("  <Border.Background>") 
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>") 
		_T("      <GradientStop Color='#527BA8' Offset='0'/>") 
		_T("      <GradientStop Color='#aac9de' Offset='0.567'/>") 
		_T("    </LinearGradientBrush>")  
		_T("  </Border.Background>")  
		_T("</Border>") 
		);

	const CString UIXAML_TIMEOD_INPUT_FALSE
		(
		_T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Input </TextBlock>")    
		//_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Stratum(Tijp)</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_TIMEOD_INPUT_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'> 통행수단 자료 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_INPUT_DEFAULT 
		(
		_T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")    
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Input </TextBlock>")    
		//_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333'> Stratum(Tijp)</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_INPUT_DEFAULT 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'> 통행수단 자료 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_INPUT_SELECTED 
		(
		_T("<Border Width='116' Height='88' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Input </TextBlock>")    
		//_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366'> Stratum(Tijp)</TextBlock>") 
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_INPUT_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'> 통행수단 자료 </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_DEFAULT 
		(
		_T("<Border Width='160' Height='102' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_DEFAULT 
		(
		_T("<Border Width='160' Height='102' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#663333'>시간대별 O/D 생성 모형</TextBlock>")    
		_T("    <Border Width='156' Height='1' Background='#6d896a' />")
		_T("    <Border Width='156' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_SELECTED 
		(
		_T("<Border Width='160' Height='102' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#6d896a' />")
		_T("    <Border Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_SELECTED 
		(
		_T("<Border Width='160' Height='102' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='12.5'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#003366'>시간대별 O/D 생성 모형</TextBlock>")    
		_T("    <Border Width='156' Height='1' Background='#6d896a' />")
		_T("    <Border Width='156' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_SETTIME_FALSE
		(
		_T("<Border Width='160' Height='71' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
// 		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'></TextBlock>")   
// 		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Characterization</TextBlock>")  
// 		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_SETTIME_DEFAULT 
		(
		_T("<Border Width='160' Height='71' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
// 		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90905'/></TextBlock>")    
// 		_T("    <Border Height='1' Background='#ddc09e' />")
// 		_T("    <Border Height='1' Background='#ffffff' />")
// 		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_SETTIME_SELECTED 
		(
		_T("<Border Width='160' Height='71' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
// 		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 10, 5, 10'> <Image Source='90905'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#6d896a' />")
// 		_T("    <Border Height='1' Background='#a1c19c' />")
// 		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_ZONECHARACTER_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Zone </TextBlock>")   
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Characterization</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_ZONECHARACTER_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>존 특성 구분자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_ZONECHARACTER_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Zone </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333'> Characterization</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_ZONECHARACTER_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>존 특성 구분자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_ZONECHARACTER_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Zone</TextBlock>")  
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366'> Characterization</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_ZONECHARACTER_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>존 특성 구분자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_PURPOSENMODE_FALSE
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#666666'> Define </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#666666'> Purpose/Mode</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_PURPOSENMODE_FALSE
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>가구통행실태조사자료</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>통행목적/수단 코드</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_PURPOSENMODE_DEFAULT
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#663333'> Define </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#663333'> Purpose/Mode</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_PURPOSENMODE_DEFAULT
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>가구통행실태조사자료</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>통행목적/수단 코드</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_PURPOSENMODE_SELECTED 
		(
		_T("<Border Width='116' Height='40' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")     
		_T("    <TextBlock Padding='5, 3, 5, 1' FontWeight='Bold' Foreground='#003366'> Define </TextBlock>")    
		_T("    <TextBlock Padding='5, 1, 5, 5' Foreground='#003366'> Purpose/Mode</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_PURPOSENMODE_SELECTED 
		(
		_T("<Border Width='116' Height='35' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>가구통행실태조사자료</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>통행목적/수단 코드</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_OUTPUT_FALSE
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/>  </TextBlock>")    
		_T("    <Border Height='1' Background='#a0a5a8' />")
		_T("    <Border Height='1' Background='#cdd1d4' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_OUTPUT_FALSE
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/>  </TextBlock>")    
// 		_T("    <Border Height='1' Background='#a0a5a8' />")
// 		_T("    <Border Height='1' Background='#cdd1d4' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_OUTPUT_DEFAULT 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_OUTPUT_DEFAULT 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#ddc09e' />")
// 		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_TIMEOD_OUTPUT_SELECTED 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_TIMEOD_OUTPUT_SELECTED 
		(
		_T("<Border Width='120' Height='120' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>출력 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
}