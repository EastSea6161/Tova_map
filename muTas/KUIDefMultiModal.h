#pragma once
namespace UIMultiModal
{
	const CString UIXAML_MA_BACKIMAGE
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

	const CString
		UIXAML_MA_MODEL_DEFAULT 
		(
		_T("<Border Width='168' Height='115' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 10'><Image Source='90905'/></TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_MODEL_DEFAULT 
		(
		_T("<Border Width='168' Height='115' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 15, 0, 15' FontWeight='Bold' Foreground='#663333'>대중교통 통행배정 모형</TextBlock>")    
		_T("    <Border Width='164' Height='1' Background='#ddc09e' />")
		_T("    <Border Width='164' Height='1' Background='#ffffff' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_MODEL_SELECTED 
		(
		_T("<Border Width='168' Height='115' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 10'> <Image Source='90905'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#6d896a' />")
		_T("    <Border Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_MODEL_SELECTED 
		(
		_T("<Border Width='168' Height='115' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 10' FontWeight='Bold' Foreground='#003366'>대중교통 통행배정 모형</TextBlock>")    
		_T("    <Border Width='164' Height='1' Background='#6d896a' />")
		_T("    <Border Width='164' Height='1' Background='#a1c19c' />")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//mode define
	const CString
		UIXAML_MA_MODEDDEFINE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Define Mode </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_MODEDDEFINE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>수단별 기종점 통행자료</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_MODEDDEFINE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Define Mode </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_MODEDDEFINE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>수단별 기종점 통행자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_MODEDDEFINE_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#003366'> Define Mode </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_MODEDDEFINE_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>수단별 기종점 통행자료</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//trip matrix
	const CString
		UIXAML_MA_TRIP_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Assign Method </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_TRIP_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>도로 통행시간 적용 방법</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_TRIP_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Assign Method </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_TRIP_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도로 통행시간 적용 방법</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_TRIP_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#003366'> Assign Method </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_TRIP_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도로 통행시간 적용 방법</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//VDF
	const CString
		UIXAML_MA_VDF_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Volume Delay </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VDF_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>통행지체함수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);


	const CString
		UIXAML_MA_VDF_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#663333'> Volume Delay </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Function</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VDF_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>통행지체함수 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);


	const CString
		UIXAML_MA_VDF_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#003366'> Volume Delay </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#003366' HorizontalAlignment='Center'> Function</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VDF_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>통행지체함수 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Highway Options
	const CString
		UIXAML_MA_HRUNOPTIONS_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Highway </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_HRUNOPTIONS_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>도로 회전제약</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_HRUNOPTIONS_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#663333'> Highway </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#663333' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_HRUNOPTIONS_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>도로 회전제약</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_HRUNOPTIONS_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#003366'> Highway </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#003366' HorizontalAlignment='Center'> Run Options </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_HRUNOPTIONS_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>도로 회전제약</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);


	//Transit Assign Options info
	const CString
		UIXAML_MA_OPTIONSINFO_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#666666'> Tansit Assign </TextBlock>")    
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#666666' HorizontalAlignment='Center'> options Info </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_OPTIONSINFO_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>대중교통 경로선택</TextBlock>")    
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>모형 계수 설정</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_OPTIONSINFO_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#663333'> Tansit Assign </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#663333' HorizontalAlignment='Center'> options Info </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_OPTIONSINFO_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>대중교통 경로선택</TextBlock>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>모형 계수 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_OPTIONSINFO_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 1, 1, 1' FontWeight='Bold' Foreground='#003366'> Tansit Assign </TextBlock>")
		_T("    <TextBlock Padding='5, 0, 1, 1' Foreground='#003366' HorizontalAlignment='Center'> options Info </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_OPTIONSINFO_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>대중교통 경로선택</TextBlock>")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>모형 계수 설정</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//stoping criteria
	const CString
		UIXAML_MA_STOPPING_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Stopping Criteria </TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_STOPPING_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>반복수행 종료조건</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_STOPPING_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Stopping Criteria </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_STOPPING_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_STOPPING_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#003366'> Stopping Criteria </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_STOPPING_SELECTED 
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>반복수행 종료조건</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	//output

	const CString
		UIXAML_MA_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#46688D' />")
		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
// 		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
// 		_T("    <Border Height='1' Background='#46688D' />")
// 		_T("    <Border Height='1' Background='#7AA5CF' />")
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>출력 설정</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
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
		KR_UIXAML_MA_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='81' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
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

	const CString
		UIXAML_MA_VIEWTABLE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		//_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW TABLE </TextBlock>")
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> Result View </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VIEWTABLE_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>결과보기</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_VIEWTABLE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		//_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> VIEW TABLE </TextBlock>")
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> Result View </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VIEWTABLE_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>결과보기</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_VIEWLOG_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#666666'> VIEW LOG </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VIEWLOG_FALSE
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>결과보기</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString
		UIXAML_MA_VIEWLOG_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='15, 8, 5, 5' FontWeight='Bold' Foreground='#663333'> VIEW LOG </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		KR_UIXAML_MA_VIEWLOG_DEFAULT
		(
		_T("<Border Width='116' Height='29' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='나눔고딕' TextBlock.FontSize='11'>")              
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>로그 파일 보기</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
}